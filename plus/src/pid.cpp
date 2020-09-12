#include "pid.h"
#include <math.h>

#ifdef USE_SPEED_ARRAY
float               PID_Module       :: speedInc[4] = {0};
float               PID_Module       :: speedPos[4] = {0};
#endif
#ifdef USE_SPEED_VECTOR
vector<float>       PID_Module       :: speedInc(4 , 0.0);
vector<float>       PID_Module       :: speedPos(4 , 0.0);
#endif

vector<float>		PID_Module       :: pitchAngle(PID_DATA_SIZE,0.0);
vector<float>		PID_Module       :: rollAngle (PID_DATA_SIZE,0.0);
vector<float>		PID_Module       :: yawAngle  (PID_DATA_SIZE,0.0);
bool                PID_Module       :: outputPitchFlag     = true;
bool                PID_Module       :: outputRollFlag      = false;
bool                PID_Module       :: outputYawFlag       = false;
bool                PID_Module       :: outputSpeedFlags[4] = {true,true,true,true};
const float 		PID_Module	     :: INFINITE = 999.0;

void xxPID::ExpectVal(float expVal){
	this->expVal = expVal;
}

void xxPID::Kp(float Kp){
	this->kp = Kp;
	this->ki = this->kp*this->ts_ms/this->ti;
	this->kd = this->kp*this->td/this->ts_ms;
}

void xxPID::Ts(uint32_t ms){
	this->ts_ms = ms;
}

void xxPID::Ti(float ti){
	this->ti = ti;
	this->ki = this->kp*this->ts_ms/this->ti;
}

void xxPID::Td(float td){
	this->td = td;
	this->kd = this->kp*this->td/this->ts_ms;
}

void xxPID::Limitation(xxPID_Limit_t& limit){
	this->limit.p = limit.p;
	this->limit.i = limit.i;
	this->limit.d = limit.d;
}

float xxPID::ExpectVal(void){
	return this->expVal;
}

float xxPID::Kp(void){
	return this->kp; 
}

float xxPID::Ki(void){
	return this->ki;
}

float xxPID::Kd(void){
	return this->kd;
}

float xxPID::Ti(void){
	return this->ti;
}

float xxPID::Td(void){
	return this->td;
}

uint32_t xxPID::Ts(void){
	return this->ts_ms;
}

xxPID_Limit_t xxPID::Limitation(void){
	return this->limit;
}

/*=================================================================================================
 * Positional PID fomula: 
                           Ts               Td
 * U[n] = Kp * { ERR[n] + ---- * ∑ERR[n] + ---- * (ERR[n]-ERR[n-1]) }
                           Ti               Ts
 * "Ts" is the period of sample rate.
==================================================================================================*/

xxPID_Position::xxPID_Position(vector<float>& dataSource):data(dataSource),errVal(PID_POS_DATASIZE,0.0){
	data.resize(PID_POS_DATASIZE);
	errVal.resize(PID_POS_DATASIZE);
	Kp(16);
	Ti(PID_Module::INFINITE);
	Ts(10);
	Td(0.0);
}

void xxPID_Position::updateData(float new_data){
	assert(this->data.size() == PID_POS_DATASIZE);
	assert(this->errVal.size() == PID_POS_DATASIZE);
	this->data.erase(this->data.begin());
	this->data.push_back(new_data);
	this->errVal.erase(this->errVal.begin());
	this->errVal.push_back((float)(expVal-data[PID_POS_DATASIZE-1]));
}

void  xxPID_Position::P_Out(vector<float>& input){}
void  xxPID_Position::I_Out(vector<float>& input){}
void  xxPID_Position::D_Out(vector<float>& input){}

void  xxPID_Position::apply(float input,vector<float>& outputBuffer){
	updateData(input);
	P_Out(outputBuffer);
	I_Out(outputBuffer);
	D_Out(outputBuffer); 
}

void xxPID_Position::apply(float input){
	apply(input,this->output);
}

PitchPID_Position::PitchPID_Position(void):xxPID_Position(PID_Module::pitchAngle){
	this->Kp(16);
	this->Ts(10); //10ms
	this->Ti(PID_Module::INFINITE);
	this->Td(0.0);

	xxPID_Limit_t param;
	param.p = 3000;
	param.i = 500;
	param.d = 900;
	Limitation(param);
}

void PitchPID_Position::P_Out(vector<float>& output){
	float out = ( this->kp * errVal[PID_POS_DATASIZE-1] );

	output[M1] += out;
	output[M2] += out;
	output[M3] += -out;
	output[M4] += -out;
}

void PitchPID_Position::I_Out(vector<float>& output){
	static float out = 0.0; 
	
	out += ( this->ki * errVal[PID_POS_DATASIZE-1] );

	LIMIT(out,  0,this->limit.i );

	output[M1] += out;
	output[M2] += out;
	output[M3] += -out;
	output[M4] += -out;
}

void PitchPID_Position::D_Out(vector<float>& output){
	float out  = ( this->kd * errVal[PID_POS_DATASIZE-1]-errVal[PID_POS_DATASIZE-2] );
	output[M1] += out;
	output[M2] += out;
	output[M3] += -out;
	output[M4] += -out;
}

RollPID_Position::RollPID_Position(void):xxPID_Position(PID_Module::rollAngle){
	this->Kp(16);
	this->Ts(10); //10ms
	this->Ti(PID_Module::INFINITE);
	this->Td(0.0);
}

void RollPID_Position::P_Out(vector<float>& output){
	float out = ( this->kp * errVal[PID_POS_DATASIZE-1] );

	output[M1] += -out;
	output[M2] += out;
	output[M3] += out;
	output[M4] += -out;
}

void RollPID_Position::I_Out(vector<float>& output){
	static float out = 0.0; 
	out += ( this->ki * errVal[PID_POS_DATASIZE-1] );

	output[M1] += -out;
	output[M2] += out;
	output[M3] += out;
	output[M4] += -out;
}

void RollPID_Position::D_Out(vector<float>& output){
	float out  = ( this->kd * errVal[PID_POS_DATASIZE-1]-errVal[PID_POS_DATASIZE-2] );

	output[M1] += -out;
	output[M2] += out;
	output[M3] += out;
	output[M4] += -out;
}


YawPID_Position::YawPID_Position(void):xxPID_Position(PID_Module::yawAngle){

}

void YawPID_Position::P_Out(vector<float>& output){

}

void YawPID_Position::I_Out(vector<float>& output){

}

void YawPID_Position::D_Out(vector<float>& output){

}


	


	
/*=================================================================================================
 * Incremental PID fomula: 
                                       Ts              Td
 * ∆U[n] = Kp * { (ERR[n]-ERR[n-1]) + ---- * ERR[n] + ---- * (ERR[n]-2*ERR[n-1]+ERR[n-2]) }
                                       Ti              Ts
 * "Ts" is the period of sample rate.
==================================================================================================*/
xxPID_Increment::xxPID_Increment(vector<float>& dataSource):data(dataSource),errVal(3,0.0){
	dataSource.resize(3);
	errVal.resize(3);
}

void xxPID_Increment::updateData(float new_data){
	assert(this->data.size() == PID_INC_DATASIZE);
	assert(this->errVal.size() == PID_INC_DATASIZE);

	this->data.erase(this->data.begin());
	this->data.push_back(new_data);
	
	this->errVal.erase(this->errVal.begin());
	this->errVal.push_back((float)(this->expVal-this->data[PID_INC_DATASIZE-1]));
}

void xxPID_Increment::apply(float input,vector<float>& outputBuffer){
	updateData(input);
	P_Out(output);
	// this->I_Out(output);
	// this->D_Out(output);
}

void xxPID_Increment::apply(float input){
	apply(input,this->output);
}

void  xxPID_Increment::P_Out(vector<float>& output){}
void  xxPID_Increment::I_Out(vector<float>& output){}
void  xxPID_Increment::D_Out(vector<float>& output){}
/*=================================================================================
 * Proportion      --- [PITCH] Increment
 * Integration     --- [PITCH] Increment
 * Differentiation --- [PITCH] Increment
==================================================================================*/
PitchPID_Increment::PitchPID_Increment(void):xxPID_Increment(PID_Module::pitchAngle){
	this->Kp(0.0);
	this->Ts(10); //10ms
	this->Ti(PID_Module::INFINITE);
	this->Td(0.0);
}

void PitchPID_Increment::P_Out(vector<float>& output){
	float out = (float)(this->kp * (this->errVal[2]-this->errVal[1])  );

	output[1-1] += out;	
	output[2-1] += out;

	output[3-1] -= out;
	output[4-1] -= out;
}

void PitchPID_Increment::I_Out(vector<float>& output){
	float out = (float)(this->ki * this->errVal[2]   );

	output[1-1] += out;	
	output[2-1] += out;

	output[3-1] -= out;
	output[4-1] -= out;
}

void PitchPID_Increment::D_Out(vector<float>& output){
	float out = (float)(this->kd * (this->errVal[2] - 2*(this->errVal[1]) + this->errVal[0])  );

	output[1-1] += out;	
	output[2-1] += out;

	output[3-1] -= out;
	output[4-1] -= out;
}

/*================================================================================
 * Proportion      --- [ROLL] 
 * Integration     --- [ROLL]
 * Differentiation --- [ROLL]
==================================================================================*/
RollPID_Increment::RollPID_Increment(void):xxPID_Increment(PID_Module::rollAngle){
	this->Kp(0.0);
	this->Ts(10); //10ms
	this->Ti(PID_Module::INFINITE);
	this->Td(0.0);
}

void  RollPID_Increment::P_Out(vector<float>& output){
	float out = (this->kp * (this->errVal[2]-this->errVal[1])  );

	output[2-1] += out;
	output[3-1] += out;

	output[1-1] -= out;
	output[4-1] -= out;
}

void  RollPID_Increment::I_Out(vector<float>& output){
	float out = (float)(this->ki * this->errVal[2]   );

	output[2-1] += out;
	output[3-1] += out;

	output[1-1] -= out;
	output[4-1] -= out;
}

void  RollPID_Increment::D_Out(vector<float>& output){
	float out = (float)(this->kd * (this->errVal[2-1] - 2*(this->errVal[1]) + this->errVal[0])  );

	output[2-1] += out;
	output[3-1] += out;

	output[1-1] -= out;
	output[4-1] -= out;
}

/*================================================================================
 * Proportion      --- [YAW] 
 * Integration     --- [YAW]
 * Differentiation --- [YAW]
==================================================================================*/
YawPID_Increment::YawPID_Increment(void):xxPID_Increment(PID_Module::yawAngle){}

void  YawPID_Increment::P_Out(vector<float>& output){
	float out = (this->kp * (this->errVal[2]-this->errVal[1])  );

	output[1-1] += out;	
	output[3-1] += out;

	output[2-1] -= out;
	output[4-1] -= out;
}

void  YawPID_Increment::I_Out(vector<float>& output){
	float out = (this->ki * this->errVal[2]   );

}

void  YawPID_Increment::D_Out(vector<float>& output){
	float out = (this->kd * (this->errVal[2] - 2*(this->errVal[1]) + this->errVal[0])  );

}



/*=================================================================================

==================================================================================*/

void PID_Module::resetIncData(int value){
#ifdef USE_SPEED_ARRAY
	memset(speedInc,value,4);
	memset(pitch.output,value,4);
	memset(roll.output,value,4);
	memset(yaw.output,value,4);
#endif
#ifdef USE_SPEED_VECTOR
	speedInc.assign(4,value);
	pitch.output.assign(4,value);
	roll.output.assign(4,value);
	yaw.output.assign(4,value);
#endif
}

void PID_Module::resetPosData(int value){
#ifdef USE_SPEED_ARRAY
	memset(speedPos,value,4);
	memset(pitch.output,value,4);
	memset(roll.output,value,4);
	memset(yaw.output,value,4);
#endif
#ifdef USE_SPEED_VECTOR
	speedPos.assign(4,value);
	pitch.output.assign(4,value);
	roll.output.assign(4,value);
	yaw.output.assign(4,value);
#endif
}

void PID_Module::resetData(int value){
	resetIncData(value);
	resetPosData(value);
}

void PID_Module::clearIncData(void){
	resetIncData(0);
}

void PID_Module::clearPosData(void){
	resetPosData(0);
}

void PID_Module::clearData(void){
	resetData(0);
}

void PID_Module::resetSpeed(void){
	float pitch_Kp = this->pitch.Kp();
	float roll_Kp  = this->roll.Kp();
	// float yawKp   = this->yaw.Kp();

	float pitch_Pout = pitch_Kp*( pitch.ExpectVal() - pitchAngle[2] );
	float roll_Pout  = roll_Kp*( rollAngle[2] - roll.ExpectVal());

	resetPosData((int)averageSpeed);

	speedPos[1-1] += pitch_Pout;
	speedPos[2-1] += pitch_Pout; 
	speedPos[3-1] -= pitch_Pout;
	speedPos[4-1] -= pitch_Pout;

	// speedPos[2-1] += roll_Pout;
	// speedPos[3-1] += roll_Pout; 
	// speedPos[1-1] -= roll_Pout;
	// speedPos[4-1] -= roll_Pout;

}

void PID_Module::apply(struct EulerAngle_t& angle){
#ifdef USE_POSITIONAL_PID
	clearPosData();
	pitch.apply(angle.pitch);
	roll.apply(angle.roll);
	yaw.apply(angle.yaw);

	speedPos[M1] =   pitch.output[M1] * (outputPitchFlag & outputSpeedFlags[M1])\
	               + roll.output [M1] * (outputRollFlag  & outputSpeedFlags[M1]);

	speedPos[M2] =   pitch.output[M2] * (outputPitchFlag & outputSpeedFlags[M2])\
	               + roll.output [M2] * (outputRollFlag  & outputSpeedFlags[M2]);               

	speedPos[M3] =   pitch.output[M3] * (outputPitchFlag & outputSpeedFlags[M3])\
	               + roll.output [M3] * (outputRollFlag  & outputSpeedFlags[M3]);

	speedPos[M4] =   pitch.output[M4] * (outputPitchFlag & outputSpeedFlags[M4])\
	               + roll.output [M4] * (outputRollFlag  & outputSpeedFlags[M4]);


#endif


}









