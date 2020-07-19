#ifndef _NRF24L01_H
#define _NRF24L01_H 

#include "fsl_lpspi.h"
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"
#include "fsl_common.h"
#include "pad_config.h"


#define USE_LPSPI1_GROUP2
#define USE_LPSPI2_GROUP1
#define USE_LPSPI3_GROUP2 
#define USE_LPSPI4_GROUP2

#define  PCS0 	0
#define  PCS1 	1
#define  PCS2 	2
#define  PCS3 	3

#if (!defined ( LPSPI1_SCK ))||(!defined( LPSPI1_MOSI ))||(!defined( LPSPI1_MISO ))
  #if   defined USE_LPSPI1_GROUP1
	#define  LPSPI1_SCK  	IOMUXC_GPIO_EMC_27_LPSPI1_SCK
	#define  LPSPI1_MOSI 	IOMUXC_GPIO_EMC_28_LPSPI1_SDO
	#define  LPSPI1_MISO	IOMUXC_GPIO_EMC_29_LPSPI1_SDI
	#define  LPSPI1_PCS0    IOMUXC_GPIO_EMC_30_LPSPI1_PCS0
	#define  LPSPI1_PCS1    IOMUXC_GPIO_EMC_31_LPSPI1_PCS1
	#define  LPSPI1_PCS2    IOMUXC_GPIO_EMC_40_LPSPI1_PCS2
	#define  LPSPI1_PCS3    IOMUXC_GPIO_EMC_41_LPSPI1_PCS3
  #elif defined USE_LPSPI1_GROUP2
	#define  LPSPI1_SCK 	IOMUXC_GPIO_SD_B0_00_LPSPI1_SCK
	#define  LPSPI1_MOSI 	IOMUXC_GPIO_SD_B0_02_LPSPI1_SDO
	#define  LPSPI1_MISO	IOMUXC_GPIO_SD_B0_03_LPSPI1_SDI
	#define  LPSPI1_PCS0 	IOMUXC_GPIO_SD_B0_01_LPSPI1_PCS0
	#define  LPSPI1_PCS1 
	#define  LPSPI1_PCS2 
	#define  LPSPI1_PCS3 
  #else
	#warning "Ambiguous [LPSPI1] Pin Group Detected! Please Add Defination at "spi.h"
	#define  LPSPI1_SCK  	IOMUXC_GPIO_EMC_27_LPSPI1_SCK
	#define  LPSPI1_MOSI 	IOMUXC_GPIO_EMC_28_LPSPI1_SDO
	#define  LPSPI1_MISO	IOMUXC_GPIO_EMC_29_LPSPI1_SDI
	#define  LPSPI1_PCS0    IOMUXC_GPIO_EMC_30_LPSPI1_PCS0
	#define  LPSPI1_PCS1    IOMUXC_GPIO_EMC_31_LPSPI1_PCS1
	#define  LPSPI1_PCS2    IOMUXC_GPIO_EMC_40_LPSPI1_PCS2
	#define  LPSPI1_PCS3    IOMUXC_GPIO_EMC_41_LPSPI1_PCS3
  #endif
#endif

#if (!defined ( LPSPI2_SCK ))||(!defined( LPSPI2_MOSI ))||(!defined( LPSPI2_MISO ))
  #if   defined USE_LPSPI2_GROUP1
	#define  LPSPI2_SCK 	IOMUXC_GPIO_SD_B1_07_LPSPI2_SCK
	#define  LPSPI2_MOSI 	IOMUXC_GPIO_SD_B1_08_LPSPI2_SD0 //There is a spelling error in "fsl_iomuxc.h"!
	#define  LPSPI2_MISO	IOMUXC_GPIO_SD_B1_09_LPSPI2_SDI
    #define  LPSPI2_PCS0 	IOMUXC_GPIO_SD_B1_06_LPSPI2_PCS0
	#define  LPSPI2_PCS1
    #define  LPSPI2_PCS2 	IOMUXC_GPIO_SD_B1_10_LPSPI2_PCS2
    #define  LPSPI2_PCS3 	IOMUXC_GPIO_SD_B1_11_LPSPI2_PCS3


  #elif defined USE_LPSPI2_GROUP2
	#define  LPSPI2_SCK 	IOMUXC_GPIO_EMC_00_LPSPI2_SCK
	#define  LPSPI2_MOSI 	IOMUXC_GPIO_EMC_02_LPSPI2_SDO
	#define  LPSPI2_MISO	IOMUXC_GPIO_EMC_03_LPSPI2_SDI
	#define  LPSPI2_PCS0 	IOMUXC_GPIO_EMC_01_LPSPI2_PCS0
	#define  LPSPI2_PCS1 	IOMUXC_GPIO_EMC_14_LPSPI2_PCS1
	#define  LPSPI2_PCS2
	#define  LPSPI2_PCS3
  #else
	#warning "Ambiguous [LPSPI2] Pin Group Detected! Please Add Defination at "spi.h""
	#define  LPSPI2_SCK 	IOMUXC_GPIO_SD_B1_07_LPSPI2_SCK
	#define  LPSPI2_MOSI 	IOMUXC_GPIO_SD_B1_08_LPSPI2_SD0 //There is a spelling error in "fsl_iomuxc.h"!
	#define  LPSPI2_MISO	IOMUXC_GPIO_SD_B1_09_LPSPI2_SDI
    #define  LPSPI2_PCS0 	IOMUXC_GPIO_SD_B1_06_LPSPI2_PCS0
	#define  LPSPI2_PCS1
    #define  LPSPI2_PCS2 	IOMUXC_GPIO_SD_B1_10_LPSPI2_PCS2
    #define  LPSPI2_PCS3 	IOMUXC_GPIO_SD_B1_11_LPSPI2_PCS3
  #endif
#endif


#if (!defined ( LPSPI3_SCK ))||(!defined( LPSPI3_MOSI ))||(!defined( LPSPI3_MISO ))
  #if   defined USE_LPSPI3_GROUP1
	#define  LPSPI3_SCK   	IOMUXC_GPIO_AD_B0_00_LPSPI3_SCK
	#define  LPSPI3_MOSI 	IOMUXC_GPIO_AD_B0_01_LPSPI3_SDO
	#define  LPSPI3_MISO	IOMUXC_GPIO_AD_B0_02_LPSPI3_SDI
	#define  LPSPI3_PCS0 	IOMUXC_GPIO_AD_B0_03_LPSPI3_PCS0
	#define  LPSPI3_PCS1 	IOMUXC_GPIO_AD_B0_04_LPSPI3_PCS1
	#define  LPSPI3_PCS2 	IOMUXC_GPIO_AD_B0_05_LPSPI3_PCS2
	#define  LPSPI3_PCS3 	IOMUXC_GPIO_AD_B0_06_LPSPI3_PCS3
  #elif defined USE_LPSPI3_GROUP2
	#define  LPSPI3_SCK 	IOMUXC_GPIO_AD_B1_15_LPSPI3_SCK
	#define  LPSPI3_MOSI 	IOMUXC_GPIO_AD_B1_14_LPSPI3_SDO
	#define  LPSPI3_MISO 	IOMUXC_GPIO_AD_B1_13_LPSPI3_SDI
	#define  LPSPI3_PCS0 	IOMUXC_GPIO_AD_B1_12_LPSPI3_PCS0
	#define  LPSPI3_PCS1
	#define  LPSPI3_PCS2
	#define  LPSPI3_PCS3
  #else
	#warning "Ambiguous [LPSPI3] Pin Group Detected! Please Add Defination at "spi.h""
	#define  LPSPI3_SCK   	IOMUXC_GPIO_AD_B0_00_LPSPI3_SCK
	#define  LPSPI3_MOSI 	IOMUXC_GPIO_AD_B0_01_LPSPI3_SDO
	#define  LPSPI3_MISO	IOMUXC_GPIO_AD_B0_02_LPSPI3_SDI
	#define  LPSPI3_PCS0 	IOMUXC_GPIO_AD_B0_03_LPSPI3_PCS0
	#define  LPSPI3_PCS1 	IOMUXC_GPIO_AD_B0_04_LPSPI3_PCS1
	#define  LPSPI3_PCS2 	IOMUXC_GPIO_AD_B0_05_LPSPI3_PCS2
	#define  LPSPI3_PCS3 	IOMUXC_GPIO_AD_B0_06_LPSPI3_PCS3
  #endif
#endif

#if (!defined ( LPSPI4_SCK ))||(!defined( LPSPI4_MOSI ))||(!defined( LPSPI4_MISO ))
  #if   defined USE_LPSPI4_GROUP1
	#define  LPSPI4_SCK   	IOMUXC_GPIO_B0_03_LPSPI4_SCK
	#define  LPSPI4_MOSI 	IOMUXC_GPIO_B0_02_LPSPI4_SDO
	#define  LPSPI4_MISO	IOMUXC_GPIO_B0_01_LPSPI4_SDI
	#define  LPSPI4_PCS0 	IOMUXC_GPIO_B0_00_LPSPI4_PCS0	
	#define  LPSPI4_PCS1
	#define  LPSPI4_PCS2
	#define  LPSPI4_PCS3
  #elif defined USE_LPSPI4_GROUP2
	#define  LPSPI4_SCK 	IOMUXC_GPIO_B1_07_LPSPI4_SCK
	#define  LPSPI4_MOSI 	IOMUXC_GPIO_B1_06_LPSPI4_SDO
	#define  LPSPI4_MISO 	IOMUXC_GPIO_B1_05_LPSPI4_SDI
	#define  LPSPI4_PCS0 	IOMUXC_GPIO_B1_04_LPSPI4_PCS0
	#define  LPSPI4_PCS1 	IOMUXC_GPIO_B1_03_LPSPI4_PCS1
	#define  LPSPI4_PCS2 	IOMUXC_GPIO_B1_02_LPSPI4_PCS2
	#define  LPSPI4_PCS3 	IOMUXC_GPIO_B1_11_LPSPI4_PCS3
  #else
	#warning "Ambiguous [LPSPI4] Pin Group Detected! Please Add Defination at "spi.h""
	#define  LPSPI4_SCK 	IOMUXC_GPIO_B1_07_LPSPI4_SCK
	#define  LPSPI4_MOSI 	IOMUXC_GPIO_B1_06_LPSPI4_SDO
	#define  LPSPI4_MISO 	IOMUXC_GPIO_B1_05_LPSPI4_SDI
	#define  LPSPI4_PCS0 	IOMUXC_GPIO_B1_04_LPSPI4_PCS0
	#define  LPSPI4_PCS1 	IOMUXC_GPIO_B1_03_LPSPI4_PCS1
	#define  LPSPI4_PCS2 	IOMUXC_GPIO_B1_02_LPSPI4_PCS2
	#define  LPSPI4_PCS3 	IOMUXC_GPIO_B1_11_LPSPI4_PCS3	
  #endif
#endif

#define LPSPI_CLOCK_FREQUENCY (uint32_t)(CLOCK_GetFreq(kCLOCK_Usb1PllPfd0Clk)/(5+1))

#ifndef __10MHz__
#define __10MHz__  10000000U
#endif







#define SPI_PAD_CONFIG_DATA  0x10B0

#define NRF_READ_REG    0x00  //读配置寄存器,低5位为寄存器地址
#define NRF_WRITE_REG   0x20  //写配置寄存器,低5位为寄存器地址
#define RD_RX_PLOAD     0x61  //读RX有效数据,1~32字节
#define WR_TX_PLOAD     0xA0  //写TX有效数据,1~32字节
#define FLUSH_TX        0xE1  //清除TX FIFO寄存器.发射模式下用
#define FLUSH_RX        0xE2  //清除RX FIFO寄存器.接收模式下用
#define REUSE_TX_PL     0xE3  //重新使用上一包数据,CE为高,数据包被不断发送.
#define NOP             0xFF  //空操作,可以用来读状态寄存器	 
//SPI(NRF24L01)寄存器地址
#define CONFIG          0x00  //配置寄存器地址;bit0:1接收模式,0发射模式;bit1:电选择;bit2:CRC模式;bit3:CRC使能;
                              //bit4:中断MAX_RT(达到最大重发次数中断)使能;bit5:中断TX_DS使能;bit6:中断RX_DR使能
#define EN_AA           0x01  //使能自动应答功能  bit0~5,对应通道0~5
#define EN_RXADDR       0x02  //接收地址允许,bit0~5,对应通道0~5
#define SETUP_AW        0x03  //设置地址宽度(所有数据通道):bit1,0:00,3字节;01,4字节;02,5字节;
#define SETUP_RETR      0x04  //建立自动重发;bit3:0,自动重发计数器;bit7:4,自动重发延时 250*x+86us
#define RF_CH           0x05  //RF通道,bit6:0,工作通道频率;
#define RF_SETUP        0x06  //RF寄存器;bit3:传输速率(0:1Mbps,1:2Mbps);bit2:1,发射功率;bit0:低噪声放大器增益
#define STATUS          0x07  //状态寄存器;bit0:TX FIFO满标志;bit3:1,接收数据通道号(最大:6);bit4,达到最多次重发
                              //bit5:数据发送完成中断;bit6:接收数据中断;
#define MAX_TX  		0x10  //达到最大发送次数中断
#define TX_OK   		0x20  //TX发送完成中断
#define RX_OK   		0x40  //接收到数据中断

#define OBSERVE_TX      0x08  //发送检测寄存器,bit7:4,数据包丢失计数器;bit3:0,重发计数器
#define CD              0x09  //载波检测寄存器,bit0,载波检测;
#define RX_ADDR_P0      0x0A  //数据通道0接收地址,最大长度5个字节,低字节在前
#define RX_ADDR_P1      0x0B  //数据通道1接收地址,最大长度5个字节,低字节在前
#define RX_ADDR_P2      0x0C  //数据通道2接收地址,最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等;
#define RX_ADDR_P3      0x0D  //数据通道3接收地址,最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等;
#define RX_ADDR_P4      0x0E  //数据通道4接收地址,最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等;
#define RX_ADDR_P5      0x0F  //数据通道5接收地址,最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等;
#define TX_ADDR         0x10  //发送地址(低字节在前),ShockBurstTM模式下,RX_ADDR_P0与此地址相等
#define RX_PW_P0        0x11  //接收数据通道0有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P1        0x12  //接收数据通道1有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P2        0x13  //接收数据通道2有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P3        0x14  //接收数据通道3有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P4        0x15  //接收数据通道4有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P5        0x16  //接收数据通道5有效数据宽度(1~32字节),设置为0则非法
#define NRF_FIFO_STATUS 0x17  //FIFO状态寄存器;bit0,RX FIFO寄存器空标志;bit1,RX FIFO满标志;bit2,3,保留
                              //bit4,TX FIFO空标志;bit5,TX FIFO满标志;bit6,1,循环发送上一数据包.0,不循环;
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//24L01操作线
#define NRF24L01_CE(n)     (n?GPIO_PinWrite(this->CE_GPIOx,this->CE_PINx,1):GPIO_PinWrite(this->CE_GPIOx,this->CE_PINx,0))   //24L01片选信号
#define NRF24L01_CSN(n)    (n?GPIO_PinWrite(this->CS_GPIOx,this->CS_PINx,1):GPIO_PinWrite(this->CS_GPIOx,this->CS_PINx,0))   //SPI片选信号	
#define NRF24L01_IRQ        GPIO_PinRead(this->IRQ_GPIOx,this->IRQ_PINx)  //IRQ主机数据输入

//24L01发送接收数据宽度定义
#define TX_ADR_WIDTH    5   	//5字节的地址宽度
#define RX_ADR_WIDTH    5   	//5字节的地址宽度
#define TX_PLOAD_WIDTH  32  	//32字节的用户数据宽度
#define RX_PLOAD_WIDTH  32  	//32字节的用户数据宽度
				

class NRF24L01{
public:
	NRF24L01(void){}
	NRF24L01(	LPSPI_Type* LPSPIx,uint8_t PCSx,\
				GPIO_Type* CE_GPIOx ,uint8_t CE_PINx,\
				GPIO_Type* IRQ_GPIOx,uint8_t IRQ_PINx,\
				GPIO_Type* CS_GPIOx ,uint8_t CS_PINx):\
	rx_addr{0xff,0xff,0xff,0xff,0xff},tx_addr{0xff,0xff,0xff,0xff,0xff},\
	rx_buffer{0},tx_buffer{0} {
		assert(CE_PINx < 32);
		assert(IRQ_PINx< 32);
		assert(CS_PINx < 32);
		this->LPSPIx 	= LPSPIx;
		this->CE_GPIOx  = CE_GPIOx;
		this->CE_PINx	= CE_PINx;
		this->IRQ_GPIOx = IRQ_GPIOx;
		this->IRQ_PINx  = IRQ_PINx;
		this->CS_GPIOx  = CS_GPIOx;
		this->CS_PINx	= CS_PINx;
		this->IOMUXC_MUX_PAD_RESOURCE_Config(NULL);
		
	}
	void init(LPSPI_Type* LPSPIx,uint8_t PCSx,\
				GPIO_Type* CE_GPIOx ,uint8_t CE_PINx,\
				GPIO_Type* IRQ_GPIOx,uint8_t IRQ_PINx,\
				GPIO_Type* CS_GPIOx ,uint8_t CS_PINx){
		NRF24L01(LPSPIx,PCSx,CE_GPIOx,CE_PINx,IRQ_GPIOx,IRQ_PINx,CS_GPIOx,CS_PINx);
	}
	status_t    check(void);
	status_t    rxMode(void);
	status_t    txMode(void);
	status_t    recv(void);
	status_t 	send(void);
	status_t    send(uint8_t* buffer,size_t num);
	uint8_t		tx_buffer[32];
	uint8_t     rx_buffer[32];
private:
	
	GPIO_Type* 	CE_GPIOx;
	GPIO_Type* 	IRQ_GPIOx;
	GPIO_Type* 	CS_GPIOx;
	uint8_t 	CE_PINx;
	uint8_t 	IRQ_PINx;
	uint8_t 	CS_PINx;
	status_t    writeReg(uint8_t reg,uint8_t  data);
	status_t    writeReg(uint8_t reg,uint8_t* data,size_t num);
	status_t    readReg(uint8_t  reg,uint8_t* data);
	status_t    readReg(uint8_t  reg,uint8_t* data,size_t num);
	uint8_t	 	tx_addr[TX_ADR_WIDTH]; 
	uint8_t 	rx_addr[RX_ADR_WIDTH];
	
	LPSPI_Type* LPSPIx;
	void 		IOMUXC_MUX_PAD_RESOURCE_Config(const lpspi_master_config_t* p);
	status_t    readwriteByte(uint8_t* mosi_data,uint8_t* miso_data);
};





#endif		