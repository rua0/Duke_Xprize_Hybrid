//Author: Yao
//Email: yy123@duke.edu
//this is mid-level operations that controls the engine and provides an interface
//engine object gives access to throttle and choke and esc
//controller object is an abstraction of the feedback loop to calculate the output

#ifndef _MYCONTROLLER_H
#define _MYCONTROLLER_H

#include "myfunc.h"
#include "myGloab.h"

extern Adafruit_PWMServoDriver pwm;

class ControllerTypeDef{
public:
	ControllerTypeDef();

private:

};

//read input voltage and convert to current

class EngineTypeDef{
public:
	EngineTypeDef();
	void init();//initialize low-level stuffs
	void acq();//acquire sensor data
	void exe();//execute by updating servo values
	// void calc();//calculate servo values
	void update();//update servo values

private:
	double thr, chk, ESC, tmp;
	int STAGE,prev_STAGE;
	int tach, bat_cur_out;
};

#endif 
