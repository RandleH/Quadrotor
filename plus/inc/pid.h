#ifndef _PID_H
#define _PID_H 
#include <iostream>
#include <vector>
#include "jy901s.h"
#include "motor.h"
#define PID_DEBUG_CONSOLE
//===================================================================
// Ç(t)  = Cur(t)-Exp(t)	
// Ç’(t) = Cur(t)-Exp(t) - (Last(t) + Exp(t)) = Cur(t) - Last(t)
//===================================================================
// 纯P调节:
// m(t) = Kp * Ç(t)
// 纯I调节:
// m(t) = Ki * ∫Ç(t)dt:[0~t] = Ki * ∑Ç(t)
// 纯D调节:
// m(t) = Kd*Ç’(t)
//===================================================================
// Kd = Kp * τ; 
// Ki = Kp / Ti;
//===================================================================
// PD调节:          
// m(t) = Kp * Ç(t) + Kp*τ*Ç’(t)

#if defined PID_DEBUG_CONSOLE
  #include "fsl_debug_console.h"
#endif

#define USE_POSITIONAL_PID

#ifdef USE_INCREMENTAL_PID
#undef USE_POSITIONAL_PID
#else
#define USE_POSITIONAL_PID
#endif

#define PID_DATA_SIZE 	   10
#define PID_POS_DATASIZE   2
#define PID_INC_DATASIZE   3

#define CURRENT_DATA			(PID_DATA_SIZE-1)
#define UNIFY_KP(Kp,Ti,Td,Ts)	Kp,((float)Ts/Ti),((float)Td/Ts)

using namespace std;

typedef struct{
	float  p;
	float  i;
	float  d;
}xxPID_Limit_t;

class xxPID{
public:
	xxPID():output(4,0.0){}
	void     ExpectVal(float expVal);
	void     Kp(float Kp);
	void     Ti(float ti);
	void     Td(float td);
	void     Ts(uint32_t ms);
	void     Limitation(xxPID_Limit_t&);
   
	float    ExpectVal(void);
	float    Kp(void);
	float    Ki(void);
	float    Kd(void);
	float    Ti(void);
	float    Td(void);
	uint32_t Ts(void);
	xxPID_Limit_t Limitation(void);
	vector<float>  output;//内部buffer
protected:
	float    expVal;
	float    kp;
	float    ti;
	float    ki;
	float    td;
	float    kd;
	uint32_t ts_ms;

	xxPID_Limit_t limit;
};

class xxPID_Position:public xxPID{
public:
	xxPID_Position(vector<float>& dataSource);
	void  init(void);
	void  apply(float input , vector<float>& outputBuffer);//指定一个输出buffer，并将结果存储于此。
	void  apply(float input);//默认为内部buffer，并将结果存储于此。
	bool outputSpeedFlags[4];//电机输出许可（0=计算但不加载到PID总输出；1=计算并加载到PID总输出）
protected:
	vector<float>& data;//数据源。
	vector<float>  errVal;//经计算后，记录误差数据。

	void  updateData(float new_data);//更新数据，记录新数据，并计算误差数据

	virtual void P_Out(vector<float>&);//比例调节输出（随具体模块...）
	virtual void I_Out(vector<float>&);//积分调节输出（随具体模块...）
	virtual void D_Out(vector<float>&);//微分调节输出（随具体模块...）
};

class xxPID_Increment:public xxPID{
public:
	xxPID_Increment(vector<float>& dataSource);
	void  init(void);
	void  apply(float input,vector<float>& outputBuffer);//指定一个输出buffer，并将结果存储于此。
	void  apply(float input);//默认为内部buffer，并将结果存储于此。
protected:
	vector<float>& data;//数据源。
	vector<float>  errVal;//经计算后，记录误差数据。

	void  updateData(float new_data);//更新数据，记录新数据，并计算误差数据

	virtual void P_Out(vector<float>& output);//比例调节输出（随具体模块...）
	virtual void I_Out(vector<float>& output);//积分调节输出（随具体模块...）
	virtual void D_Out(vector<float>& output);//微分调节输出（随具体模块...）
};

class PitchPID_Position:public xxPID_Position{
private:
	void P_Out(vector<float>& output);
	void I_Out(vector<float>& output);
	void D_Out(vector<float>& output);
public:
	PitchPID_Position(void);
};

class RollPID_Position:public xxPID_Position{
private:
	void P_Out(vector<float>& output);
	void I_Out(vector<float>& output);
	void D_Out(vector<float>& output);
public:
	RollPID_Position(void);
};

class YawPID_Position:public xxPID_Position{
private:
	void P_Out(vector<float>& output);
	void I_Out(vector<float>& output);
	void D_Out(vector<float>& output);
public:
	YawPID_Position(void);
};

class PitchPID_Increment:public xxPID_Increment{
private:
	void P_Out(vector<float>& output);
	void I_Out(vector<float>& output);
	void D_Out(vector<float>& output);
public:
	PitchPID_Increment(void);
};

class RollPID_Increment:public xxPID_Increment{
private:
	void P_Out(vector<float>& output);
	void I_Out(vector<float>& output);
	void D_Out(vector<float>& output);
public:
	RollPID_Increment(void);
};

class YawPID_Increment:public xxPID_Increment{
private:
	void P_Out(vector<float>& output);
	void I_Out(vector<float>& output);
	void D_Out(vector<float>& output);
public:
	YawPID_Increment(void); 
};

class PID_Module{
friend class PitchPID_Position;
friend class RollPID_Position;
friend class YawPID_Position;
friend class PitchPID_Increment;
friend class RollPID_Increment;
friend class YawPID_Increment;
public:
#ifdef USE_INCREMENTAL_PID
	PitchPID_Increment pitch;
	RollPID_Increment  roll;
  	YawPID_Increment   yaw;
#endif

#ifdef USE_POSITIONAL_PID
  	PitchPID_Position pitch;
	RollPID_Position  roll;
    YawPID_Position   yaw;
#endif

public:
	static bool outputPitchFlag;
	static bool outputRollFlag;
	static bool outputYawFlag;
private:
	static vector<float> pitchAngle;
	static vector<float> rollAngle;
	static vector<float> yawAngle;
public:
#ifdef USE_SPEED_ARRAY
	static float speedPos[4];
	static float speedInc[4];
#endif
#ifdef USE_SPEED_VECTOR
	static vector<float> speedPos;
	static vector<float> speedInc;
#endif
public:
	PID_Module(void):resetSpeedFlag(true),averageSpeed(0){}
	void clearIncData(void);
	void clearPosData(void);
	void clearData(void);
	void apply(struct EulerAngle_t& angle);
	void resetSpeed(void);
public:
  	static const float INFINITE;

  	bool   resetSpeedFlag;
  	static bool   outputSpeedFlags[4];
  	float  averageSpeed;
 private:
 	inline void resetIncData(int value);
 	inline void resetPosData(int value);
 	inline void resetData(int value);
};



#endif