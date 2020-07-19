#include "nrf24l01.h"
#include "delay.h"

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
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_11_GPIO1_IO11,0);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_04_GPIO2_IO20,0x10B0);    //NRF_CS
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_08_GPIO2_IO24,0x10B0);    //NRF_CE
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_09_GPIO2_IO25,0xF080);
    gpio_pin_config_t io_config;
    io_config.direction     = kGPIO_DigitalOutput;
	io_config.interruptMode = kGPIO_NoIntmode;
	io_config.outputLogic   = 1;
	GPIO_PinInit(this->CE_GPIOx ,this->CE_PINx  ,&io_config);
	GPIO_PinInit(this->CS_GPIOx ,this->CS_PINx  ,&io_config);
	io_config.direction        = kGPIO_DigitalInput;
	GPIO_PinInit(this->IRQ_GPIOx,this->IRQ_PINx ,&io_config);
	NRF24L01_CSN(1);
	NRF24L01_CE(0);
}

status_t NRF24L01::writeReg(uint8_t reg,uint8_t data){
	NRF24L01_CSN(0);
	this->readwriteByte(&reg  ,NULL);
	this->readwriteByte(&data ,NULL);
	NRF24L01_CSN(1);
	return kStatus_Success;
}

status_t NRF24L01::writeReg(uint8_t  reg,uint8_t* data,size_t num){
	status_t result = kStatus_Fail;
	NRF24L01_CSN(0);
	this->readwriteByte(&reg  ,NULL);
	for(size_t i=0;i<num;i++){
		result = this->readwriteByte(data,NULL);
		data++;
	}
	NRF24L01_CSN(1);
	return result;
}

status_t NRF24L01::readReg(uint8_t  reg,uint8_t* data){
	NRF24L01_CSN(0);
	this->readwriteByte(&reg  ,NULL);
	this->readwriteByte(NULL  ,data);
	NRF24L01_CSN(1);
	return kStatus_Success;
}

status_t NRF24L01::readReg(uint8_t  reg,uint8_t* data,size_t num){
	status_t result = kStatus_Fail;
	NRF24L01_CSN(0);
	this->readwriteByte(&reg  ,NULL);
	for(size_t i=0;i<num;i++){
		result = this->readwriteByte(NULL,&data[i]);
	}
	NRF24L01_CSN(1);
	return result;
}

status_t NRF24L01::readwriteByte(uint8_t* mosi_data,uint8_t* miso_data){
	uint8_t txData = 0xff,rxData = 0;
	lpspi_transfer_t transfer;
	transfer.configFlags = (kLPSPI_MasterPcs1|kLPSPI_MasterPcsContinuous); 
	if(mosi_data!=NULL)		transfer.txData      = mosi_data;
	else					transfer.txData      = &txData;		
	if(miso_data!=NULL)		transfer.rxData      = miso_data;
	else					transfer.rxData   	 = &rxData;
	transfer.dataSize    = 1;
	return LPSPI_MasterTransferBlocking(this->LPSPIx,&transfer);
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
	return kStatus_Success;
}

status_t NRF24L01::recv(void){
	uint8_t currentStatus = 0;
	this->readReg(STATUS,&currentStatus);
	this->writeReg(NRF_WRITE_REG+STATUS,currentStatus);
	if(currentStatus & RX_OK){
		this->readReg(RD_RX_PLOAD,this->rx_buffer,RX_PLOAD_WIDTH);
		this->writeReg(FLUSH_RX,0xff);//Clear RX FIFO
		return kStatus_Success;
	}
	return kStatus_NoTransferInProgress;
}

status_t NRF24L01::send(void){
	return this->send(this->tx_buffer,TX_PLOAD_WIDTH);
}

status_t NRF24L01::send(uint8_t* buffer,size_t num){
	uint8_t currentStatus = 0;
	NRF24L01_CE(0);
	this->writeReg(WR_TX_PLOAD,buffer,32);
	NRF24L01_CE(1);
	while(NRF24L01_IRQ != 0);
	this->readReg(STATUS,&currentStatus);
	this->writeReg(NRF_WRITE_REG+STATUS,currentStatus);
	if(currentStatus & MAX_TX)
		return kStatus_Timeout;
	if(currentStatus & TX_OK)
		return kStatus_Success;
	return kStatus_Fail;
}
















