#include "jy901s.h"
#include "mathematic.h"

#ifdef SYSTEM_UART_DEBUG_CONSOLE
  #include "fsl_debug_console.h"
#endif

uint8_t buffer[30] = {0};

status_t JY901::init(LPI2C_Type* LPI2Cx){
	this->I2C::init(LPI2Cx,JY901_DEFAULT_ADDRESS,__100KHz__); 
#ifdef SYSTEM_UART_DEBUG_CONSOLE
		PRINTF("Gyro Scope:\tInitialized completedly.\n");
#endif
	return kStatus_Success;
}

#include "fsl_iomuxc.h"
#include "fsl_lpuart.h"
#include "nvic.h"
#include "board.h"
status_t JY901::init(LPUART_Type* LPUARTx){
	IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_10_LPUART2_RX, 0U);                                      
  	IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_11_LPUART2_TX, 0U); 
	IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_10_LPUART2_RX, UART_RX_PAD_CONFIG_DATA);
	IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_11_LPUART2_TX, UART_TX_PAD_CONFIG_DATA);
	lpuart_config_t config;
  
	/*调用固件库函数得到默认的串口配置参数，在默认的配置参数基础上修改*/
	LPUART_GetDefaultConfig(&config);
	config.baudRate_Bps = 9600U;  //波特率
	config.enableRx = true; //是否允许接收数据
	config.enableTx = true;   //是否允许发送数据

	/*调用固件库函数，将修改好的配置信息写入到串口的配置寄存器中*/
	LPUART_Init(LPUART2, &config, BOARD_DebugConsoleSrcFreq());


	/*允许接收中断*/
	LPUART_EnableInterrupts(LPUART2, kLPUART_RxDataRegFullInterruptEnable);

	/*设置中断优先级,*/
	set_IRQn_Priority(LPUART2_IRQn,Group4_PreemptPriority_6, Group4_SubPriority_0);
	/*使能中断*/
	EnableIRQ(LPUART2_IRQn);
	return kStatus_Success;
}

status_t JY901::updateData(void){
	status_t result = this->readReg(AX,&buffer[0],24);

	this->acceler.Ax  = (float)(CHAR2SHORT(&buffer[0])/32768.0*16);
	this->acceler.Ay  = (float)(CHAR2SHORT(&buffer[2])/32768.0*16);
	this->acceler.Az  = (float)(CHAR2SHORT(&buffer[4])/32768.0*16);
	
	this->speed.Wx  = (float)(CHAR2SHORT(&buffer[6])/32768.0*2000);
	this->speed.Wy  = (float)(CHAR2SHORT(&buffer[8])/32768.0*2000);
	this->speed.Wz  = (float)(CHAR2SHORT(&buffer[10])/32768.0*2000);

	this->magnet.Bx   = (CHAR2SHORT(&buffer[12]));
	this->magnet.By   = (CHAR2SHORT(&buffer[14]));
	this->magnet.Bz   = (CHAR2SHORT(&buffer[16]));

	this->angle.pitch = (float)(((buffer[19]<<8)|buffer[18])/32768.0*180);
	this->angle.roll  = (float)(((buffer[21]<<8)|buffer[20])/32768.0*180);
	this->angle.yaw   = (float)(((buffer[23]<<8)|buffer[22])/32768.0*180);

	if(this->angle.pitch > 180.0)
		this->angle.pitch = this->angle.pitch - 360.0;
	if(this->angle.roll > 180.0)
		this->angle.roll = this->angle.roll - 360.0;
	if(this->angle.yaw > 180.0)
		this->angle.yaw = this->angle.yaw - 360.0;

	this->angle.pitch_s = (short)this->angle.pitch;
	this->angle.roll_s  = (short)this->angle.roll;
	this->angle.yaw_s   = (short)this->angle.yaw; 

	return result;
}

#ifdef _JY901_DEMO_ENABLE
#include "lcd.h"
#include "delay.h"


#endif

#ifdef __cplusplus
extern "C"{
#endif

void LPUART2_IRQHandler(void){
	static unsigned char ucRxCnt = 0;
	if ( (kLPUART_RxDataRegFullFlag)&LPUART_GetStatusFlags(LPUART2) ){
		buffer[ucRxCnt++] = LPUART_ReadByte(LPUART2);
	}
	if (buffer[0]!=0x55){
		ucRxCnt=0;
		return;
	}
	if (ucRxCnt<11) 
		return;
	else{
		switch(buffer[1]){
			case 0x50:	break;
			case 0x51:	break;
			case 0x52:	break;
			case 0x53:	break;
			case 0x54:	break;
			case 0x55:	break;
			case 0x56:	break;
			case 0x57:	break;
			case 0x58:	break;
			case 0x59:	break;
		}
		ucRxCnt=0;
	}
		
	//LPUART_ClearStatusFlags()
}


#ifdef __cplusplus
}
#endif







