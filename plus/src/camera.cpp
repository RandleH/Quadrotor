#include "camera.h"
#include "lcd.h"
#if defined(__cplusplus)
extern "C" {
#endif
#include "fsl_sccb.h"
#include "fsl_common.h"
#if defined(__cplusplus)
}
#endif

AT_NONCACHEABLE_SECTION_ALIGN(pixel_t ov5640_buffer[2][800][480],64);


OV5640::OV5640(void):CAMERA(LPI2C1){
	this->bytesPerPixel = 2;
	this->param     = ov5640_param_640_480;
	//this->resource.sccbI2C           = LPI2C1;
    //this->resource.pullResetPin      = OV5640::reset;
    //this->resource.pullPowerDownPin  = OV5640::pwr;
    //this->resource.inputClockFreq_Hz = 24000000;

	this->camera_cfg.pixelFormat   = kVIDEO_PixelFormatRGB565;
	this->camera_cfg.bytesPerPixel = this->bytesPerPixel;
	this->camera_cfg.resolution    = FSL_VIDEO_RESOLUTION(this->param.cam_out_width,this->param.cam_out_height);
	this->camera_cfg.frameBufferLinePitch_Bytes = OV5640_FRAME_WIDTH * this->bytesPerPixel;
	this->camera_cfg.interface     = kCAMERA_InterfaceGatedClock;
	this->camera_cfg.controlFlags  = (kCAMERA_HrefActiveLow | kCAMERA_DataLatchOnRisingEdge);
	this->camera_cfg.framePerSec   = 30;
    
    this->csi_resource.csiBase = CSI;
	this->camera_recv_handle.resource    = &this->csi_resource;
	this->camera_recv_handle.ops         = &csi_ops;//Defination: fsl_csi_camera_adapter.c/.h
	this->camera_recv_handle.privateData = &this->csi_privateData;//Not Used.
    CAMERA_RECEIVER_Init(&this->camera_recv_handle,&this->camera_cfg ,NULL ,NULL );
  //CSI_ADAPTER_Init    (&this->camera_recv_handle,&this->camera_cfg ,NULL ,NULL );
    this->init();
#if 1
    memset(ov5640_buffer,0,sizeof(ov5640_buffer));
    CAMERA_RECEIVER_SubmitEmptyBuffer(&this->camera_recv_handle,(uint32_t)ov5640_buffer[0]);
  //CSI_ADAPTER_SubmitEmptyBuffer    (&this->camera_recv_handle,(uint32_t)ov5640_buffer[0]);
    CAMERA_RECEIVER_SubmitEmptyBuffer(&this->camera_recv_handle,(uint32_t)ov5640_buffer[1]);
  //CSI_ADAPTER_SubmitEmptyBuffer    (&this->camera_recv_handle,(uint32_t)ov5640_buffer[1]);
#endif
    CAMERA_RECEIVER_Start(&this->camera_recv_handle);
  //CSI_ADAPTER_Start    (&this->camera_recv_handle);
    while (kStatus_Success != CAMERA_RECEIVER_GetFullBuffer(&this->camera_recv_handle, &this->activeFrameAddr));
  //while (kStatus_Success != CSI_ADAPTER_GetFullBuffer    (&this->camera_recv_handle, &this->activeFrameAddr));
    while (kStatus_Success != CAMERA_RECEIVER_GetFullBuffer(&this->camera_recv_handle, &this->inactiveFrameAddr));
  //while (kStatus_Success != CSI_ADAPTER_GetFullBuffer    (&this->camera_recv_handle, &this->inactiveFrameAddr));
}

void OV5640::reset(bool pullUp){
    if(pullUp)
        GPIO_PinWrite(CAMERA_RST_GPIO, CAMERA_RST_GPIO_PIN, 1);
    else
        GPIO_PinWrite(CAMERA_RST_GPIO, CAMERA_RST_GPIO_PIN, 0);
}

void OV5640::pwr(bool pullUp){
	if (pullUp)
	    GPIO_PinWrite(CAMERA_PWR_GPIO, CAMERA_PWR_GPIO_PIN, 1);
	else
	    GPIO_PinWrite(CAMERA_PWR_GPIO, CAMERA_PWR_GPIO_PIN, 0);
}

void OV5640::delay(uint32_t ms){
	while (ms--){
		for (volatile int i = 0U; i < 10000000U; i++)
			__ASM("nop");
	}
}

void OV5640::setFrameRate(void){
    if(this->param.frame_rate == OV5640_FRAME_RATE_30FPS){
        this->writeReg(0x3035,0x21);
        this->writeReg(0x3036,0x72);
    }
    else if(this->param.frame_rate == OV5640_FRAME_RATE_15FPS){
        this->writeReg(0x3035,0x41);
        this->writeReg(0x3036,0x72);
    }
}

void OV5640::setImgSize(uint16_t x_st,uint16_t y_st,uint16_t width,uint16_t height){
    this->writeReg(0x3800,x_st>>8);//X_ADDR_ST
    this->writeReg(0x3801,x_st&0xff);//X_ADDR_ST 
    this->writeReg(0x3802,y_st>>8);//Y_ADDR_ST¸
    this->writeReg(0x3803,y_st&0xff);//Y_ADDR_ST

    this->writeReg(0x3804,(x_st+width)>>8);//X_ADDR_END
    this->writeReg(0x3805,(x_st+width)&0xff);//X_ADDR_END 
    this->writeReg(0x3806,(y_st+height)>>8);//Y_ADDR_END
    this->writeReg(0x3807,(y_st+height)&0xff);//Y_ADDR_END
}

void OV5640::setOutSize(uint8_t scaling,uint16_t x_off,uint16_t y_off,uint16_t width,uint16_t height){
    uint8_t reg_temp;
    this->readReg(0x5001,&reg_temp);    

    if(scaling == 0)
        this->writeReg(0x5001,reg_temp &~(1<<5) );  //scaling off       
    else
        this->writeReg(0x5001,reg_temp|(1<<5) );    //scaling on         

    this->writeReg(0x3810,x_off>>8);//X offset
    this->writeReg(0x3811,x_off&0xff);//X offset
    this->writeReg(0x3812,y_off>>8);//Y offset
    this->writeReg(0x3813,y_off&0xff);//Y offset


    this->writeReg(0X3212,0X03);    

    this->writeReg(0x3808,width>>8); 
    this->writeReg(0x3809,width&0xff); 
    this->writeReg(0x380a,height>>8);
    this->writeReg(0x380b,height&0xff);

    this->writeReg(0X3212,0X13);        
    this->writeReg(0X3212,0Xa3);
}

void OV5640::brightness(int8_t value){
    this->writeReg(0x3212, 0x03); // start group 3

    this->writeReg(0x5587, (value<<4)&0xf0);

    if(value >= 0) 
        this->writeReg(0x5588, 0x01);
    else
        this->writeReg(0x5588, 0x09);
        
    this->writeReg(0x3212, 0x13); // end group 3
    this->writeReg(0x3212, 0xa3); // launch group 3
}

void OV5640::saturation(int8_t value){
    this->writeReg(0x3212, 0x03); // start group 3
    this->writeReg(0x5381, 0x1c);   
    this->writeReg(0x5382, 0x5a);
    this->writeReg(0x5383, 0x06);

    for(uint8_t i=0;i<6;i++)
        this->writeReg(0x5384+i, OV5640_saturationRegs[value+3][i]);

    this->writeReg(0x538b, 0x98);
    this->writeReg(0x538a, 0x01);   
    this->writeReg(0x3212, 0x13); // end group 3
    this->writeReg(0x3212, 0xa3); // launch group 3
}

void OV5640::contrast(int8_t value){
    //(value+3) %= 7-1;

    this->writeReg(0x3212, 0x03); // start group 3
    this->writeReg(0x5586, OV5640_contrastRegs[value+3][0]);
    this->writeReg(0x5585, OV5640_contrastRegs[value+3][1]);
    this->writeReg(0x3212, 0x13); // end group 3
    this->writeReg(0x3212, 0xa3); // launch group 3
}

void OV5640::exposure(int8_t value){
    //(value+3) %= 7-1;

    this->writeReg(0x3212, 0x03); // start group 3

    this->writeReg(0x3a0f, OV5640_exposureRegs[value+3][0]);
    this->writeReg(0x3a10, OV5640_exposureRegs[value+3][1]);
    this->writeReg(0x3a11, OV5640_exposureRegs[value+3][2]);
    this->writeReg(0x3a1b, OV5640_exposureRegs[value+3][3]);
    this->writeReg(0x3a1e, OV5640_exposureRegs[value+3][4]);
    this->writeReg(0x3a1f, OV5640_exposureRegs[value+3][5]);
    
    this->writeReg(0x3212, 0x13); // end group 3
    this->writeReg(0x3212, 0xa3); // launch group 3
}

void OV5640::lightMode(uint8_t mode){
    this->writeReg(0x3212, 0x03); // start group 3

    this->writeReg(0x3406, OV5640_lightModeRegs[mode][0]);
    this->writeReg(0x3400, OV5640_lightModeRegs[mode][1]);
    this->writeReg(0x3401, OV5640_lightModeRegs[mode][2]);
    this->writeReg(0x3402, OV5640_lightModeRegs[mode][3]);
    this->writeReg(0x3403, OV5640_lightModeRegs[mode][4]);
    this->writeReg(0x3404, OV5640_lightModeRegs[mode][5]);
    this->writeReg(0x3405, OV5640_lightModeRegs[mode][6]);

    this->writeReg(0x3212, 0x13); // end group 3
    this->writeReg(0x3212, 0xa3); // lanuch group 3
}

void OV5640::specialEffect(uint8_t effect){
    this->writeReg(0x3212, 0x03); // start group 3

    this->writeReg(0x5580, OV5640_effectRegs[effect][0]);
    this->writeReg(0x5583, OV5640_effectRegs[effect][1]); // sat U
    this->writeReg(0x5584, OV5640_effectRegs[effect][2]); // sat V
    this->writeReg(0x5003, OV5640_effectRegs[effect][3]);

    this->writeReg(0x3212, 0x13); // end group 3
    this->writeReg(0x3212, 0xa3); // launch group 3
}

uint8_t OV5640::constantFocus(void){
    uint8_t  state = 0x8F;
    uint32_t iteration = 300;
    //send constant focus mode command to firmware
    this->writeReg(0x3023,0x01);
    this->writeReg(0x3022,0x04);

    iteration = 5000;
    do{
         this->readReg(0x3023,&state);
        if (iteration-- == 0)
            return 1;
        this->delay(1);
    } while(state!=0x00);
    return 0;
}

status_t OV5640::readReg(uint32_t reg,uint8_t* data){
	return SCCB_ReadReg(this->LPI2Cx, OV5640_SCCB_ADDR, kSCCB_RegAddr16Bit, (reg), (data));
}

status_t OV5640::writeReg(uint32_t reg,uint8_t data){
	return SCCB_WriteReg(this->LPI2Cx, OV5640_SCCB_ADDR,kSCCB_RegAddr16Bit, (reg), (data));
}

status_t OV5640::writeReg(const OV5640_Reg_Data srcs[], size_t num){
    status_t status = kStatus_Success;
    for (uint32_t i = 0; i < num; i++){	
        status = this->writeReg(srcs[i].reg,srcs[i].data);
        if (kStatus_Success != status)
            break;
    }
    return status;
}

status_t OV5640::writeFirmWork(uint8_t *pBuffer ,uint16_t BufferSize){
    return SCCB_WriteMultiRegs(this->LPI2Cx, OV5640_SCCB_ADDR, kSCCB_RegAddr16Bit,0x8000,pBuffer,BufferSize);
}

status_t OV5640::init(void){
	status_t status;
	if (   (kCAMERA_InterfaceNonGatedClock != this->camera_cfg.interface) 
		&& (kCAMERA_InterfaceGatedClock    != this->camera_cfg.interface) 
		&& (kCAMERA_InterfaceCCIR656       != this->camera_cfg.interface))
        return kStatus_InvalidArgument;
    this->pwr(true);
	this->reset(false);
	this->delay(1);
    this->pwr(false);
    this->delay(2);
    this->reset(true);
    this->delay(2);

    uint8_t pid;
    status = this->readReg(OV5640_PID_REG,&pid);
    if(status != kStatus_Success)
    	return status;

    uint8_t ver;
    status = this->readReg(OV5640_VER_REG,&ver);
    if(status != kStatus_Success)
    	return status;

    if (OV5640_REVISION != (((uint32_t)pid << 8U) | (uint32_t)ver))
        return kStatus_Fail;

    this->softwareReset();
	this->delay(2);

	status = this->writeReg(OV5640_initRegs,OV5640_NumOfInitRegs);
	if (status != kStatus_Success)
        return status;

//User Config

    this->setFrameRate();
    this->delay(2);

    this->setImgSize(this->param.cam_isp_sx,
                     this->param.cam_isp_sy,
                     this->param.cam_isp_width,
                     this->param.cam_isp_height);
    this->delay(1);

    this->setOutSize(this->param.scaling,
                     this->param.cam_out_sx,
                     this->param.cam_out_sy,
                     this->param.cam_out_width,
                     this->param.cam_out_height);
    this->delay(1);

    this->brightness(this->param.brightness);
    this->delay(1);

    this->saturation(this->param.saturation);
    this->delay(1);

    this->contrast(this->param.contrast);
    this->delay(1);

    this->exposure(this->param.exposure);
    this->delay(1);

    this->lightMode(this->param.light_mode);
    this->delay(1);

    this->specialEffect(this->param.effect);
    this->delay(1);

//Auto Focus

    this->initAutoFocus();

	return kStatus_Success;
}

status_t OV5640::initExt(camera_device_handle_t *handle, const camera_config_t *config, const void *specialConfig){
	return 0;
}

status_t OV5640::initAutoFocus(void){
    uint8_t  state     = 0x8F;
    uint32_t iteration = 100;
    
    this->delay(1);
    this->writeReg(0x3000, 0x20);

    this->writeFirmWork(OV5640_AF_FW,OV5640_NumOfAF_FW);

    this->writeReg(0x3022, 0x00);
    this->writeReg(0x3023, 0x00);
    this->writeReg(0x3024, 0x00);
    this->writeReg(0x3025, 0x00);
    this->writeReg(0x3026, 0x00);
    this->writeReg(0x3027, 0x00);
    this->writeReg(0x3028, 0x00);
    this->writeReg(0x3029, 0xFF);
    this->writeReg(0x3000, 0x00);
    this->writeReg(0x3004, 0xFF);
    this->writeReg(0x0000U, 0x00);
    this->writeReg(0x0000U, 0x00);
    this->writeReg(0x0000U, 0x00);
    this->writeReg(0x0000U, 0x00);
        
    do{
        this->readReg(0x3029,&state);
        if (iteration-- == 0) 
            return kStatus_Fail;
    }while(state!=0x70);

    this->delay(1);
    this->constantFocus();
    return kStatus_Success;    
}

status_t OV5640::start(camera_device_handle_t *handle){
	return kStatus_Success;
}

status_t OV5640::stop(camera_device_handle_t *handle){
	return 0;
}

status_t OV5640::softwareReset(void){
    return this->writeReg(0x3008,0x80);
}



#ifdef __cplusplus
extern "C"{
#endif

void CSI_IRQHandler(void)
{
    CSI_DriverIRQHandler();
     
}

#ifdef __cplusplus
}
#endif

