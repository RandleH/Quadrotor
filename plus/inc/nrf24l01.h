#ifndef _NRF24L01_H
#define _NRF24L01_H 

#include "fsl_lpspi.h"
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"
#include "fsl_common.h"
#include "pad_config.h"

#ifdef SYSTEM_UART_DEBUG_CONSOLE
  #include "fsl_debug_console.h"
#endif


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



#define RADIO_IRQn           	GPIO2_Combined_16_31_IRQn
#define RADIO_IRQHandler    	GPIO2_Combined_16_31_IRQHandler

#define IRQ_PAD_CONFIG_DATA            (SRE_0_SLOW_SLEW_RATE| \
                                        DSE_0_OUTPUT_DRIVER_DISABLED| \
                                        SPEED_2_MEDIUM_100MHz| \
                                        ODE_0_OPEN_DRAIN_DISABLED| \
                                        PKE_1_PULL_KEEPER_ENABLED| \
                                        PUE_1_PULL_SELECTED| \
                                        PUS_3_22K_OHM_PULL_UP| \
                                        HYS_1_HYSTERESIS_ENABLED) 

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
// SPI寄存器配置     Registers Config

// 0x00 CONFIG  配置寄存器
#define NRF_RCD_RX_DR_EN            0<<6    // bit6=MASK_RX_DR=0=映射RX_DR中断(数据接收完成时IRQ输出L)
#define NRF_RCD_RX_DR_DI            1<<6    // bit6=MASK_RX_DR=1=屏蔽RX_DR中断
#define NRF_RCD_TX_DS_EN            0<<5    // bit5=MASK_TX_DR=0=映射TX_DR中断(数据接收完成时IRQ输出L)
#define NRF_RCD_TX_DS_DI            1<<5    // bit5=MASK_TX_DR=1=屏蔽TX_DR中断
#define NRF_RCD_MAX_RT_EN           0<<4    // bit4=MASK_MAX_RT=0=映射MASK_MAX_RT中断(超过重发上限时IRQ输出L)
#define NRF_RCD_MAX_RT_DI           1<<4    // bit4=MASK_MAX_RT=1=屏蔽MASK_MAX_RT中断
#define NRF_RCD_CRC_DI              0<<3    // bit3=EN_CRC=0=禁用CRC
#define NRF_RCD_CRC_EN              1<<3    // bit3=EN_CRC=1=启用CRC
#define NRF_RCD_CRC8                0<<2    // bit2=CRCO=0=CRC-8=1byte
#define NRF_RCD_CRC16               1<<2    // bit2=CRCO=1=CRC-16=2byte
#define NRF_RCD_PWR_DOWN            0<<1    // bit1=PWR_UP=0=电源关闭模式
#define NRF_RCD_PWR_UP              1<<1    // bit1=PWR_UP=1=电源开启模式
#define NRF_RCD_PRIM_PRX            1       // bit0=PRIM_RX=1=接收模式
#define NRF_RCD_PRIM_PTX            0       // bit0=PRIM_RX=0=发送模式

// 0x01 EN_AA   使能自动应答
#define NRF_RCD_ALL_PIPE_AA_DI      0       // bit5:0=000000=禁用全部通道的AA
#define NRF_RCD_PIPE5_AA_DI         0<<5    // bit5=ENAA_P5=0=禁用通道5的AA(Auto Ack自动应答)
#define NRF_RCD_PIPE5_AA_EN         1<<5    // bit5=ENAA_P5=1=启用通道5的AA
#define NRF_RCD_PIPE4_AA_DI         0<<4    // bit4=ENAA_P4=0=禁用通道4的AA
#define NRF_RCD_PIPE4_AA_EN         1<<4    // bit4=ENAA_P4=1=启用通道4的AA
#define NRF_RCD_PIPE3_AA_DI         0<<3    // bit3=ENAA_P3=0=禁用通道3的AA
#define NRF_RCD_PIPE3_AA_EN         1<<3    // bit3=ENAA_P3=1=启用通道3的AA
#define NRF_RCD_PIPE2_AA_DI         0<<2    // bit2=ENAA_P2=0=禁用通道2的AA
#define NRF_RCD_PIPE2_AA_EN         1<<2    // bit2=ENAA_P2=1=启用通道2的AA
#define NRF_RCD_PIPE1_AA_DI         0<<1    // bit1=ENAA_P1=0=禁用通道1的AA
#define NRF_RCD_PIPE1_AA_EN         1<<1    // bit1=ENAA_P1=1=启用通道1的AA
#define NRF_RCD_PIPE0_AA_DI         0       // bit0=ENAA_P0=0=禁用通道0的AA
#define NRF_RCD_PIPE0_AA_EN         1       // bit0=ENAA_P0=1=启用通道0的AA

// 0x02 EN_RXADDR   使能RX数据通道
#define NRF_RCD_PIPE5_RX_DI         0<<5    // bit5=ERX_P5=0=禁用通道5
#define NRF_RCD_PIPE5_RX_EN         1<<5    // bit5=ERX_P5=1=启用通道5
#define NRF_RCD_PIPE4_RX_DI         0<<4    // bit4=ERX_P4=0=禁用通道4
#define NRF_RCD_PIPE4_RX_EN         1<<4    // bit4=ERX_P4=1=启用通道4
#define NRF_RCD_PIPE3_RX_DI         0<<3    // bit3=ERX_P3=0=禁用通道3
#define NRF_RCD_PIPE3_RX_EN         1<<3    // bit3=ERX_P3=1=启用通道3
#define NRF_RCD_PIPE2_RX_DI         0<<2    // bit2=ERX_P2=0=禁用通道2
#define NRF_RCD_PIPE2_RX_EN         1<<2    // bit2=ERX_P2=1=启用通道2
#define NRF_RCD_PIPE1_RX_DI         0<<1    // bit1=ERX_P1=0=禁用通道1
#define NRF_RCD_PIPE1_RX_EN         1<<1    // bit1=ERX_P1=1=启用通道1
#define NRF_RCD_PIPE0_RX_DI         0       // bit0=ERX_P0=0=禁用通道0
#define NRF_RCD_PIPE0_RX_EN         1       // bit0=ERX_P0=1=启用通道0

// 0x03 SETUP_AW    设置地址宽度
#define NRF_RCD_AW_3byte            1       // bit1:0=01=地址宽度为3byte
#define NRF_RCD_AW_4byte            2       // bit1:0=10=地址宽度为4byte
#define NRF_RCD_AW_5byte            3       // bit1:0=11=地址宽度为5byte

// 0x04 SETUP_RETR  设置自动重发
#define NRF_RCD_ARD_250us           0<<4    // bit7:4=0000=自动重发延时为250us
#define NRF_RCD_ARD_500us           1<<4    // bit7:4=0001=自动重发延时为500us
#define NRF_RCD_ARD_750us           2<<4    // bit7:4=0010=自动重发延时为750us
#define NRF_RCD_ARD_1000us          3<<4    // bit7:4=0011=自动重发延时为1000us
#define NRF_RCD_ARD_1250us          4<<4    // bit7:4=0100=自动重发延时为1250us
#define NRF_RCD_ARD_1500us          5<<4    // bit7:4=0101=自动重发延时为1500us
#define NRF_RCD_ARD_1750us          6<<4    // bit7:4=0110=自动重发延时为1750us
#define NRF_RCD_ARD_2000us          7<<4    // bit7:4=0111=自动重发延时为2000us
#define NRF_RCD_ARD_2250us          8<<4    // bit7:4=1000=自动重发延时为2250us
#define NRF_RCD_ARD_2500us          9<<4    // bit7:4=1001=自动重发延时为2500us
#define NRF_RCD_ARD_2750us          10<<4   // bit7:4=1010=自动重发延时为2750us
#define NRF_RCD_ARD_3000us          11<<4   // bit7:4=1011=自动重发延时为3000us
#define NRF_RCD_ARD_3250us          12<<4   // bit7:4=1100=自动重发延时为3250us
#define NRF_RCD_ARD_3500us          13<<4   // bit7:4=1101=自动重发延时为3500us
#define NRF_RCD_ARD_3750us          14<<4   // bit7:4=1110=自动重发延时为3750us
#define NRF_RCD_ARD_4000us          15<<4   // bit7:4=1111=自动重发延时为4000us
#define NRF_RCD_ARC_DI              0       // bit3:0=0000=禁用自动重发
#define NRF_RCD_ARC_1               1       // bit3:0=0001=自动重发1次
#define NRF_RCD_ARC_2               2       // bit3:0=0010=自动重发2次
#define NRF_RCD_ARC_3               3       // bit3:0=0011=自动重发3次
#define NRF_RCD_ARC_4               4       // bit3:0=0100=自动重发4次
#define NRF_RCD_ARC_5               5       // bit3:0=0101=自动重发5次
#define NRF_RCD_ARC_6               6       // bit3:0=0110=自动重发6次
#define NRF_RCD_ARC_7               7       // bit3:0=0111=自动重发7次
#define NRF_RCD_ARC_8               8       // bit3:0=1000=自动重发8次
#define NRF_RCD_ARC_9               9       // bit3:0=1001=自动重发9次
#define NRF_RCD_ARC_10              10      // bit3:0=1010=自动重发10次
#define NRF_RCD_ARC_11              11      // bit3:0=1011=自动重发11次
#define NRF_RCD_ARC_12              12      // bit3:0=1100=自动重发12次
#define NRF_RCD_ARC_13              13      // bit3:0=1101=自动重发13次
#define NRF_RCD_ARC_14              14      // bit3:0=1110=自动重发14次
#define NRF_RCD_ARC_15              15      // bit3:0=1111=自动重发15次

// 0x05 RF_CH   频道设置    (Freq=2400+RF_CH[MHz],2400~2525MHz)
#define NRF_RCD_RF_CH_max           125     // bit6:0   工作频率最大值
#define NRF_RCD_RF_CH_min           0       // bit6:0   工作频率最小值

// 0x06 RF_SETUP    RF设置
#define NRF_RCD_CONT_WAVE_DI        0<<7    // bit7=CONT_WAVE=0=禁用连续载波传送
#define NRF_RCD_CONT_WAVE_EN        1<<7    // bit7=CONT_WAVE=1=启用连续载波传送
#define NRF_RCD_RF_DR_250kbps       4<<3    // bit5&bit3=10=250kbps
#define NRF_RCD_RF_DR_1Mbps         0<<3    // bit5&bit3=00=1Mbps
#define NRF_RCD_RF_DR_2Mbps         1<<3    // bit5&bit3=01=2Mbps
#define NRF_RCD_PLL_LOCK_DI         0<<4    // bit4=PLL_LOCK=0=不锁定PLL
#define NRF_RCD_PLL_LOCK_EN         1<<4    // bit4=PLL_LOCK=1=锁定PLL
#define NRF_RCD_RF_PWR_0dBm         3<<1    // bit2:1=RF_PWR=11=0dBm
#define NRF_RCD_RF_PWR_n6dBm        2<<1    // bit2:1=RF_PWR=10=-6dBm
#define NRF_RCD_RF_PWR_n12dBm       1<<1    // bit2:1=RF_PWR=01=-12dBm
#define NRF_RCD_RF_PWR_n18dBm       0<<1    // bit2:1=RF_PWR=00=-18dBm

// 0x07 STATUS  状态
#define NRF_RCD_RX_DR               1<<6    // bit6=1=RX_DR=数据接收完成中断(写1清除)
#define NRF_RCD_TX_DR               1<<5    // bit5=1=TX_DR=数据发送完成中断(写1清除)
#define NRF_RCD_MAX_RT              1<<4    // bit4=1=MAX_RT=重发上限中断(写1清除)
#define NRF_RCD_RX_PIPE0_DR         0<<1    // bit3:1=000=接收到的数据包为通道0
#define NRF_RCD_RX_PIPE1_DR         1<<1    // bit3:1=001=接收到的数据包为通道1
#define NRF_RCD_RX_PIPE2_DR         2<<1    // bit3:1=010=接收到的数据包为通道2
#define NRF_RCD_RX_PIPE3_DR         3<<1    // bit3:1=011=接收到的数据包为通道3
#define NRF_RCD_RX_PIPE4_DR         4<<1    // bit3:1=100=接收到的数据包为通道4
#define NRF_RCD_RX_PIPE5_DR         5<<1    // bit3:1=101=接收到的数据包为通道5
#define NRF_RCD_RX_FIFO_EMPTY       7<<1    // bit3:1=111=RX FIFO为空
#define NRF_RCD_TX_FIFO_FULL        1       // bit0=1=TX FIFO已满

// 0x08 OBSERVE_TX  发送监测
#define NRF_RCD_PLOS_CNT_MAX        15      // bit7:4   丢包计数器最大值    
#define NRF_RCD_PLOS_CNT_MIN        0       // bit7:4   丢包计数器最小值
#define NRF_RCD_ARC_CNT_MAX         15      // bit3:0   重发计数器最大值    
#define NRF_RCD_ARC_CNT_MIN         0       // bit3:0   重发计数器最小值

// 0x09 RPD 接收功率监测
#define NRF_RCD_RPD_n64dBm_UP       1       // bit0=1=接收功率>=-64dBm
#define NRF_RCD_RPD_n64dbm_DOWN     0       // bit0=0=接收功率<-64dBm

// 0x11 RX_PW_PX    接收通道X数据宽度
#define NRF_RCD_RX_PW_PX_MAX        32      // bit5:0    通道X数据宽度最大值
#define NRF_RCD_RX_PW_PX_MIN        1       // bit5:0    通道X数据宽度最小值
#define NRF_RCD_RX_PW_PX_DI         0       // bit5:0    通道X未使用

// 0x17 FIFO_STATUS FIFO状态
#define NRF_RCD_TX_REUSE            1<<6    // bit6=1=设备处于重发模式
#define NRF_RCD_TX_FIFO_S_FULL      1<<5    // bit5=1=TX FIFO已满
#define NRF_RCD_TX_FIFO_S_EMPTY     1<<4    // bit4=1=TX FIFO无数据
#define NRF_RCD_RX_FIFO_S_FULL      1<<1    // bit1=1=RX FIFO已满
#define NRF_RCD_RX_FIFO_S_EMPTY     1       // bit0=1=RX FIFO无数据

// 0x1C Enable dynamic payload length   使能动态数据包长度
#define NRF_RCD_PIPE5_DPL_EN        1<<5    // bit5=1=开启通道5的动态数据包长度
#define NRF_RCD_PIPE5_DPL_DI        0<<5    // bit5=0=关闭通道5的动态数据包长度
#define NRF_RCD_PIPE4_DPL_EN        1<<4    // bit4=1=开启通道4的动态数据包长度
#define NRF_RCD_PIPE4_DPL_DI        0<<4    // bit4=0=关闭通道4的动态数据包长度
#define NRF_RCD_PIPE3_DPL_EN        1<<3    // bit3=1=开启通道3的动态数据包长度
#define NRF_RCD_PIPE3_DPL_DI        0<<3    // bit3=0=关闭通道3的动态数据包长度
#define NRF_RCD_PIPE2_DPL_EN        1<<2    // bit2=1=开启通道2的动态数据包长度
#define NRF_RCD_PIPE2_DPL_DI        0<<2    // bit2=0=关闭通道2的动态数据包长度
#define NRF_RCD_PIPE1_DPL_EN        1<<1    // bit1=1=开启通道1的动态数据包长度
#define NRF_RCD_PIPE1_DPL_DI        0<<1    // bit1=0=关闭通道1的动态数据包长度
#define NRF_RCD_PIPE0_DPL_EN        1       // bit0=1=开启通道0的动态数据包长度
#define NRF_RCD_PIPE0_DPL_DI        0       // bit0=0=关闭通道0的动态数据包长度

// 0x1D Feature Register    特征寄存器
#define NRF_RCD_DPL_EN              1<<2    // bit2=1=允许动态数据包长度
#define NRF_RCD_DPL_DI              0<<2    // bit2=0=禁止动态数据包长度
#define NRF_RCD_ACK_PAY_EN          1<<1    // bit1=1=允许带ACK的数据
#define NRF_RCD_ACK_PAY_DI          0<<1    // bit1=0=禁止带ACK的数据
#define NRF_RCD_DYN_ACK_PAY_EN      1       // bit1=1=允许无ACK的数据
#define NRF_RCD_DYN_ACK_PAY_DI      0       // bit1=0=禁止无ACK的数据



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

typedef enum{
	kMasterMode,
	kSlaveMode
}RadioDirection_t;
				

class NRF24L01{
public:
	NRF24L01(void):rx_addr{0xff,0xff,0xff,0xff,0xff},tx_addr{0xff,0xff,0xff,0xff,0xff},\
					rx_buffer{0},tx_buffer{0}{}
	
	void init(	LPSPI_Type* LPSPIx	,uint8_t PCSx,\
				GPIO_Type* CE_GPIOx ,uint8_t CE_PINx,\
				GPIO_Type* IRQ_GPIOx,uint8_t IRQ_PINx,\
				GPIO_Type* CS_GPIOx ,uint8_t CS_PINx);
	status_t    check(void);
	status_t    rxMode(void);
	status_t    txMode(void);
	status_t    recv(void);
	status_t 	send(void);
	status_t    send(uint8_t* buffer,size_t num);
	status_t    clearIRQ(void);
	uint8_t		tx_buffer[32];
	uint8_t     rx_buffer[32];
	static volatile bool	recvFlag;
	RadioDirection_t direction;
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