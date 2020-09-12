#include "nrf24l01.h"
#include "delay.h"
#include "nvic.h"

volatile bool			NRF24L01::recvFlag   = false; 
static volatile bool*	EXTERN_C_recvFlag	 = &NRF24L01::recvFlag;
#ifdef __cplusplus
extern "C"{
#endif
static LPSPI_Type* 	EXTERN_C_LPSPIx		= NULL;		
static GPIO_Type*  	EXTERN_C_CE_GPIOx	= NULL;
static uint8_t		EXTERN_C_CE_PINx	= 32;
static GPIO_Type*  	EXTERN_C_CS_GPIOx	= NULL;
static uint8_t		EXTERN_C_CS_PINx	= 32;
static GPIO_Type*  	EXTERN_C_IRQ_GPIOx	= NULL;
static uint8_t		EXTERN_C_IRQ_PINx	= 32;
static uint8_t*		EXTERN_C_rxBuffer	= NULL;

static status_t 	EXTERN_C_rwByte(LPSPI_Type* LPSPIx,uint8_t* mosi_data,uint8_t* miso_data);
static status_t 	EXTERN_C_wReg(LPSPI_Type* LPSPIx,GPIO_Type* PCS_GPIOx,uint8_t PCS_PINx,uint8_t reg,uint8_t data);
static status_t 	EXTERN_C_wRegs(LPSPI_Type* LPSPIx,GPIO_Type* PCS_GPIOx,uint8_t PCS_PINx,uint8_t reg,uint8_t* data,size_t num);
static status_t 	EXTERN_C_rReg(LPSPI_Type* LPSPIx,GPIO_Type* PCS_GPIOx,uint8_t PCS_PINx,uint8_t  reg,uint8_t* data);
static status_t 	EXTERN_C_rRegs(LPSPI_Type* LPSPIx,GPIO_Type* PCS_GPIOx,uint8_t PCS_PINx,uint8_t  reg,uint8_t* data,size_t num);
static status_t 	EXTERN_C_nrfIRQ_Handler(void);
#ifdef __cplusplus
}
#endif

void NRF24L01::init(LPSPI_Type* LPSPIx,uint8_t PCSx,\
				GPIO_Type* CE_GPIOx ,uint8_t CE_PINx,\
				GPIO_Type* IRQ_GPIOx,uint8_t IRQ_PINx,\
				GPIO_Type* CS_GPIOx ,uint8_t CS_PINx){
	assert(CE_PINx < 32);
	assert(IRQ_PINx< 32);
	assert(CS_PINx < 32);

//API for "C"
	EXTERN_C_LPSPIx		= this->LPSPIx 	  = LPSPIx;
	EXTERN_C_CE_GPIOx	= this->CE_GPIOx  = CE_GPIOx;
	EXTERN_C_CE_PINx	= this->CE_PINx   = CE_PINx;
	EXTERN_C_CS_GPIOx	= this->CS_GPIOx  = CS_GPIOx;
	EXTERN_C_CS_PINx	= this->CS_PINx   = CS_PINx;
	EXTERN_C_IRQ_GPIOx  = this->IRQ_GPIOx = IRQ_GPIOx;
	EXTERN_C_IRQ_PINx   = this->IRQ_PINx  = IRQ_PINx;
	EXTERN_C_rxBuffer  	= this->rx_buffer;
	this->IOMUXC_MUX_PAD_RESOURCE_Config(NULL);
#ifdef SYSTEM_UART_DEBUG_CONSOLE
	PRINTF("Radio:\tInitialized completedly.\n");
#endif
}

void NRF24L01::IOMUXC_MUX_PAD_RESOURCE_Config(const lpspi_master_config_t* p){
	switch((uint32_t)this->LPSPIx){
		case LPSPI1_BASE:		IOMUXC_SetPinMux   (LPSPI1_SCK , 1U);                                   
								IOMUXC_SetPinMux   (LPSPI1_MOSI, 1U);
								IOMUXC_SetPinMux   (LPSPI1_MISO, 1U);
								IOMUXC_SetPinConfig(LPSPI1_SCK , SPI_PAD_CONFIG_DATA);                               
								IOMUXC_SetPinConfig(LPSPI1_MOSI, SPI_PAD_CONFIG_DATA);
								IOMUXC_SetPinConfig(LPSPI1_MISO, SPI_PAD_CONFIG_DATA);break;
			case LPSPI2_BASE:	IOMUXC_SetPinMux   (LPSPI2_SCK , 1U);                                   
								IOMUXC_SetPinMux   (LPSPI2_MOSI, 1U);
								IOMUXC_SetPinMux   (LPSPI2_MISO, 1U);
								IOMUXC_SetPinConfig(LPSPI2_SCK , SPI_PAD_CONFIG_DATA);                               
								IOMUXC_SetPinConfig(LPSPI2_MOSI, SPI_PAD_CONFIG_DATA);
								IOMUXC_SetPinConfig(LPSPI2_MISO, SPI_PAD_CONFIG_DATA);break;
			case LPSPI3_BASE:	IOMUXC_SetPinMux   (LPSPI3_SCK , 1U);                                   
								IOMUXC_SetPinMux   (LPSPI3_MOSI, 1U);
								IOMUXC_SetPinMux   (LPSPI3_MISO, 1U);
								IOMUXC_SetPinConfig(LPSPI3_SCK , SPI_PAD_CONFIG_DATA);                               
								IOMUXC_SetPinConfig(LPSPI3_MOSI, SPI_PAD_CONFIG_DATA);
								IOMUXC_SetPinConfig(LPSPI3_MISO, SPI_PAD_CONFIG_DATA);break;
			case LPSPI4_BASE:	IOMUXC_SetPinMux   (LPSPI4_SCK , 1U);                                   
								IOMUXC_SetPinMux   (LPSPI4_MOSI, 1U);
								IOMUXC_SetPinMux   (LPSPI4_MISO, 1U);
								IOMUXC_SetPinConfig(LPSPI4_SCK , SPI_PAD_CONFIG_DATA);                               
								IOMUXC_SetPinConfig(LPSPI4_MOSI, SPI_PAD_CONFIG_DATA);
								IOMUXC_SetPinConfig(LPSPI4_MISO, SPI_PAD_CONFIG_DATA);break;
	}
	CLOCK_SetMux(kCLOCK_LpspiMux,1);    //USB1 PLL PFD0(576MHz)作为LPSPI时钟源
    CLOCK_SetDiv(kCLOCK_LpspiDiv,5);
    lpspi_master_config_t config;
	if(p==NULL){
		config.baudRate			  = __10MHz__;               //SPI速度
		config.whichPcs			  = kLPSPI_Pcs0;                 //片选信号，PCS0
		config.pcsActiveHighOrLow = kLPSPI_PcsActiveLow;//片选信号低电平有效
		config.bitsPerFrame		  = 8;                       //设置SPI的数据大小:SPI发送
		config.cpol 			  = kLPSPI_ClockPolarityActiveHigh;
		config.cpha 			  = kLPSPI_ClockPhaseFirstEdge;     
		config.direction 		  = kLPSPI_MsbFirst;            //指定数据传输从MSB位还是LSB
		config.pinCfg 			  = kLPSPI_SdiInSdoOut;            //SDI输入引脚，SDO输出引脚
		config.dataOutConfig 	  = kLpspiDataOutRetained;  //输出数据保留
		config.pcsToSckDelayInNanoSec 	     = 10;            //片选拉低到时钟有效之间的延
		config.lastSckToPcsDelayInNanoSec    = 10;        //最后一个时钟到片选拉高之间
		config.betweenTransferDelayInNanoSec = 10;     //两次传输之间的延时，单位ns
		p = &config;
	}
	LPSPI_MasterInit(this->LPSPIx,p,LPSPI_CLOCK_FREQUENCY);
    LPSPI_Enable(this->LPSPIx,true);  

    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_04_GPIO2_IO20,0);   //NRF_CS
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_08_GPIO2_IO24,0);   //NRF_CE
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_09_GPIO2_IO25,0);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_04_GPIO2_IO20,0x10B0);    //NRF_CS
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_08_GPIO2_IO24,0x10B0);    //NRF_CE
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_09_GPIO2_IO25,IRQ_PAD_CONFIG_DATA );
    gpio_pin_config_t io_config;
    io_config.direction     = kGPIO_DigitalOutput;
	io_config.interruptMode = kGPIO_NoIntmode;
	io_config.outputLogic   = 1;
	GPIO_PinInit(this->CE_GPIOx ,this->CE_PINx  ,&io_config);
	GPIO_PinInit(this->CS_GPIOx ,this->CS_PINx  ,&io_config);
	io_config.direction        = kGPIO_DigitalInput;
	io_config.interruptMode    = kGPIO_IntLowLevel;
	IRQ_GPIOx = this->IRQ_GPIOx;
	IRQ_PINx  = this->IRQ_PINx;
	//GPIO_PinInit(GPIO2,25U,&io_config);
	//GPIO_PortEnableInterrupts(GPIO2, 1U << 25);
	GPIO_PinInit(this->IRQ_GPIOx,this->IRQ_PINx ,&io_config);
	GPIO_PortEnableInterrupts(this->IRQ_GPIOx,1U << this->IRQ_PINx);
	
	NRF24L01_CSN(1);
	NRF24L01_CE(0);

}

status_t NRF24L01::writeReg(uint8_t reg,uint8_t data){

	return EXTERN_C_wReg(this->LPSPIx,this->CS_GPIOx,this->CS_PINx,reg,data);
}

status_t NRF24L01::writeReg(uint8_t  reg,uint8_t* data,size_t num){

	return EXTERN_C_wRegs(this->LPSPIx,this->CS_GPIOx,this->CS_PINx,reg,data,num);
}

status_t NRF24L01::readReg(uint8_t  reg,uint8_t* data){

	return  EXTERN_C_rReg(this->LPSPIx,this->CS_GPIOx,this->CS_PINx,reg,data);
}

status_t NRF24L01::readReg(uint8_t  reg,uint8_t* data,size_t num){

	return EXTERN_C_rRegs(this->LPSPIx,this->CS_GPIOx,this->CS_PINx,reg,data,num);
}

status_t NRF24L01::readwriteByte(uint8_t* mosi_data,uint8_t* miso_data){

	return EXTERN_C_rwByte(this->LPSPIx,mosi_data,miso_data);
}

status_t NRF24L01::check(void){
	uint8_t buf[5]={0xA5,0xA5,0xA5,0xA5,0xA5};
	this->writeReg(NRF_WRITE_REG+TX_ADDR,buf,5);
	DELAY::ms(10);
	this->readReg(TX_ADDR,buf,5);
	for(uint8_t i=0;i<5;i++)
		if(buf[i]!=0xA5)
			return kStatus_Fail;
	return kStatus_Success;	 							   
}

status_t NRF24L01::rxMode(void){
	NRF24L01_CE(0);
	this->writeReg(NRF_WRITE_REG+RX_ADDR_P0 ,this->rx_addr,RX_ADR_WIDTH);//写RX节点地址
	  
  	this->writeReg(NRF_WRITE_REG+EN_AA      ,0x01);	//Auto-ACK Enable 
  	this->writeReg(NRF_WRITE_REG+EN_RXADDR  ,0x01); //RX-Channel:0 	 
  	this->writeReg(NRF_WRITE_REG+RF_CH      ,40);	//RF-Channel:40		  
  	this->writeReg(NRF_WRITE_REG+RX_PW_P0   ,RX_PLOAD_WIDTH);//Valid Data Size   
  	this->writeReg(NRF_WRITE_REG+RF_SETUP   ,0x0f);	//0db;2Mbps;LPF 
  	this->writeReg(NRF_WRITE_REG+CONFIG     ,0x0f);	//PWR Config:RX Mode
	NRF24L01_CE(1);
	GPIO_PortClearInterruptFlags(EXTERN_C_IRQ_GPIOx, 1U << EXTERN_C_IRQ_PINx);
	set_IRQn_Priority(RADIO_IRQn,Group4_PreemptPriority_6, Group4_SubPriority_0);
	EnableIRQ(RADIO_IRQn);
	return kStatus_Success;
}

status_t NRF24L01::txMode(void){
	NRF24L01_CE(0);
	this->writeReg(NRF_WRITE_REG+TX_ADDR    ,this->tx_addr ,TX_ADR_WIDTH);
	this->writeReg(NRF_WRITE_REG+RX_ADDR_P0 ,this->rx_addr ,RX_ADR_WIDTH);
	this->writeReg(NRF_WRITE_REG+EN_AA      ,0x01);	//Auto-ACK Enable
	this->writeReg(NRF_WRITE_REG+EN_RXADDR  ,0x01);	//RX-Channel:0
	this->writeReg(NRF_WRITE_REG+SETUP_RETR ,0x1a);	//500+86us x 10times
	this->writeReg(NRF_WRITE_REG+RF_CH      ,40);	//RF-Channel:40
	this->writeReg(NRF_WRITE_REG+RF_SETUP   ,0x0f);	//0db;2Mbps;LPF
	this->writeReg(NRF_WRITE_REG+CONFIG     ,0x0e);	//PWR Config:TX Mode
	NRF24L01_CE(1);
	DisableIRQ(RADIO_IRQn);
	return kStatus_Success;
}

status_t NRF24L01::recv(void){

	return EXTERN_C_nrfIRQ_Handler();
}

status_t NRF24L01::send(void){
	return this->send(this->tx_buffer,TX_PLOAD_WIDTH);
}

status_t NRF24L01::send(uint8_t* buffer,size_t num){
	uint8_t currentStatus = 0;
	NRF24L01_CE(0);
	//this->writeReg(FLUSH_TX   ,0xff);
	this->writeReg(WR_TX_PLOAD,buffer,32);
	NRF24L01_CE(1);
	//while(NRF24L01_IRQ != 0);
	this->readReg(STATUS,&currentStatus);
	this->writeReg(NRF_WRITE_REG+STATUS,currentStatus);
	if(currentStatus & MAX_TX)
		return kStatus_Timeout;
	if(currentStatus & TX_OK){
		this->writeReg(FLUSH_TX   ,0xff);
		return kStatus_Success;
	}
	return kStatus_Fail;
}

status_t NRF24L01::clearIRQ(void){
	this->IRQ_GPIOx->ISR = 1U << this->IRQ_PINx;
	return kStatus_Success;
}

#ifdef __cplusplus
extern "C"{
#endif

//extern void (*API_DELAY_ms_Handle)(uint32_t);

static status_t EXTERN_C_wReg(LPSPI_Type* LPSPIx,GPIO_Type* PCS_GPIOx,uint8_t PCS_PINx,uint8_t reg,uint8_t data){
	status_t result = kStatus_Fail;
	GPIO_PinWrite(PCS_GPIOx,PCS_PINx,0);
	result = EXTERN_C_rwByte(LPSPIx,&reg  ,NULL);
	result = EXTERN_C_rwByte(LPSPIx,&data ,NULL);
	GPIO_PinWrite(PCS_GPIOx,PCS_PINx,1);
	return result;
}

static status_t EXTERN_C_wRegs(LPSPI_Type* LPSPIx,GPIO_Type* PCS_GPIOx,uint8_t PCS_PINx,uint8_t reg,uint8_t* data,size_t num){
	status_t result = kStatus_Fail;
	GPIO_PinWrite(PCS_GPIOx,PCS_PINx,0);
	EXTERN_C_rwByte(LPSPIx,&reg  ,NULL);
	for(size_t i=0;i<num;i++){
		result = EXTERN_C_rwByte(LPSPIx,data,NULL);
		data++;
	}
	GPIO_PinWrite(PCS_GPIOx,PCS_PINx,1);
	return result;
}

static status_t EXTERN_C_rReg(LPSPI_Type* LPSPIx,GPIO_Type* PCS_GPIOx,uint8_t PCS_PINx,uint8_t  reg,uint8_t* data){
	status_t result = kStatus_Fail;
	GPIO_PinWrite(PCS_GPIOx,PCS_PINx,0);
	result = EXTERN_C_rwByte(LPSPIx,&reg  ,NULL);
	result = EXTERN_C_rwByte(LPSPIx,NULL  ,data);
	GPIO_PinWrite(PCS_GPIOx,PCS_PINx,1);
	return result;
}

static status_t EXTERN_C_rRegs(LPSPI_Type* LPSPIx,GPIO_Type* PCS_GPIOx,uint8_t PCS_PINx,uint8_t  reg,uint8_t* data,size_t num){
	status_t result = kStatus_Fail;
	GPIO_PinWrite(PCS_GPIOx,PCS_PINx,0);
	result = EXTERN_C_rwByte(LPSPIx,&reg  ,NULL);
	for(size_t i=0;i<num;i++){
		result = EXTERN_C_rwByte(LPSPIx,NULL,&data[i]);
	}
	GPIO_PinWrite(PCS_GPIOx,PCS_PINx,1);
	return result;
}

static status_t EXTERN_C_rwByte(LPSPI_Type* LPSPIx,uint8_t* mosi_data,uint8_t* miso_data){
	uint8_t txData = 0xff,rxData = 0;
	lpspi_transfer_t transfer;
	transfer.configFlags = (kLPSPI_MasterPcs1|kLPSPI_MasterPcsContinuous); 
	if(mosi_data!=NULL)		transfer.txData      = mosi_data;
	else					transfer.txData      = &txData;		
	if(miso_data!=NULL)		transfer.rxData      = miso_data;
	else					transfer.rxData   	 = &rxData;
	transfer.dataSize    = 1;
	return LPSPI_MasterTransferBlocking(LPSPIx,&transfer);
}

static status_t EXTERN_C_nrfIRQ_Handler(void){
	uint8_t currentStatus = 0;
	GPIO_PinWrite(EXTERN_C_CE_GPIOx,EXTERN_C_CE_PINx,0);
	EXTERN_C_rReg(EXTERN_C_LPSPIx,EXTERN_C_CS_GPIOx,EXTERN_C_CS_PINx,STATUS,&currentStatus);
	//this->readReg(STATUS,&currentStatus);
	EXTERN_C_wReg(EXTERN_C_LPSPIx,EXTERN_C_CS_GPIOx,EXTERN_C_CS_PINx,NRF_WRITE_REG+STATUS,currentStatus);
	//this->writeReg(NRF_WRITE_REG+STATUS,currentStatus);
	if(currentStatus & RX_OK){
		EXTERN_C_rRegs(EXTERN_C_LPSPIx,EXTERN_C_CS_GPIOx,EXTERN_C_CS_PINx,RD_RX_PLOAD,EXTERN_C_rxBuffer,RX_PLOAD_WIDTH);
		//this->readReg(RD_RX_PLOAD,this->rx_buffer,RX_PLOAD_WIDTH);
		EXTERN_C_wReg(EXTERN_C_LPSPIx,EXTERN_C_CS_GPIOx,EXTERN_C_CS_PINx,FLUSH_RX,0xff);
		//this->writeReg(FLUSH_RX,0xff);//Clear RX FIFO
		GPIO_PinWrite(EXTERN_C_CE_GPIOx,EXTERN_C_CE_PINx,1);
		*EXTERN_C_recvFlag = true;
		return kStatus_Success;
	}
	GPIO_PinWrite(EXTERN_C_CE_GPIOx,EXTERN_C_CE_PINx,1);
	return kStatus_NoTransferInProgress;
}

void GPIO2_Combined_16_31_IRQHandler(void){
	if(GPIO_PinRead(EXTERN_C_IRQ_GPIOx,EXTERN_C_IRQ_PINx) == 0){
		EXTERN_C_nrfIRQ_Handler();
	}
	GPIO_PortClearInterruptFlags(EXTERN_C_IRQ_GPIOx, 1U << EXTERN_C_IRQ_PINx);

#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}



#ifdef __cplusplus
}
#endif








