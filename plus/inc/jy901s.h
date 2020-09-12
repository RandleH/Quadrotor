#ifndef _JY901S_H
#define _JY901S_H

#include "i2c.h"
#include "fsl_lpuart.h"
#include "uart.h"

#define _JY901_DEMO_ENABLE

#ifndef __100KHz__
#define __100KHz__ 100000U
#endif
#ifndef __200KHz__
#define __200KHz__ 200000U
#endif
#ifndef __300KHz__
#define __300KHz__ 300000U
#endif
#ifndef __400KHz__
#define __400KHz__ 400000U
#endif


#define JY901_DEFAULT_ADDRESS         0x50

#define SAVE 				0x00
#define CALSW 				0x01
#define RSW 				0x02
#define RRATE				0x03
#define BAUD 				0x04
#define AXOFFSET			0x05
#define AYOFFSET			0x06
#define AZOFFSET			0x07
#define GXOFFSET			0x08
#define GYOFFSET			0x09
#define GZOFFSET			0x0a
#define HXOFFSET			0x0b
#define HYOFFSET			0x0c
#define HZOFFSET			0x0d
#define D0MODE				0x0e
#define D1MODE				0x0f
#define D2MODE				0x10
#define D3MODE				0x11
#define D0PWMH				0x12
#define D1PWMH				0x13
#define D2PWMH				0x14
#define D3PWMH				0x15
#define D0PWMT				0x16
#define D1PWMT				0x17
#define D2PWMT				0x18
#define D3PWMT				0x19
#define IICADDR				0x1a
#define LEDOFF 				0x1b
#define GPSBAUD				0x1c

#define YYMM				0x30
#define DDHH				0x31
#define MMSS				0x32
#define MS					0x33
#define AX					0x34
#define AY					0x35
#define AZ					0x36
#define GX					0x37
#define GY					0x38
#define GZ					0x39
#define HX					0x3a
#define HY					0x3b
#define HZ					0x3c			
#define Roll				0x3d
#define Pitch				0x3e
#define Yaw					0x3f
#define TEMP				0x40
#define D0Status			0x41
#define D1Status			0x42
#define D2Status			0x43
#define D3Status			0x44
#define PressureL			0x45
#define PressureH			0x46
#define HeightL				0x47
#define HeightH				0x48
#define LonL				0x49
#define LonH				0x4a
#define LatL				0x4b
#define LatH				0x4c
#define GPSHeight   		0x4d
#define GPSYAW      		0x4e
#define GPSVL				0x4f
#define GPSVH				0x50
      
#define DIO_MODE_AIN 		0
#define DIO_MODE_DIN 		1
#define DIO_MODE_DOH 		2
#define DIO_MODE_DOL 		3
#define DIO_MODE_DOPWM 		4
#define DIO_MODE_GPS 		5		

struct EulerAngle_t{
	float pitch;
	float roll;
	float yaw;
	short pitch_s;
	short roll_s;
	short yaw_s;
};

typedef struct{
	float Ax;
	float Ay;
	float Az;
}Acceleration_t;

struct AngularSpeed_t{
	float Wx;
	float Wy;
	float Wz;
};

typedef struct{
	short Bx;
	short By;
	short Bz;
}MagneticField_t;

class MotionData{
public:
	MotionData(void){}
	struct EulerAngle_t 		angle;
	struct AngularSpeed_t		speed;
	Acceleration_t      acceler;
	MagneticField_t     magnet;
};

class JY901:public I2C,public MotionData{
public:
	JY901(void){}
	//JY901(LPI2C_Type* LPI2Cx,LPUART_Type* LPUARTx = NULL):I2C(LPI2Cx,JY901_DEFAULT_ADDRESS,__100KHz__){}
	status_t init(LPI2C_Type* LPI2Cx);
	status_t init(LPUART_Type* LPUARTx);
#ifdef _JY901_DEMO_ENABLE
	static void DEMO(void);
#endif
	status_t updateData(void);
	//EulerAngle_t 		angle;
	//Acceleration_t      acceler;
	//MagneticField_t     magnet;
private:
	bool   USE_I2C_Interface;
	
};


#endif	