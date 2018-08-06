#ifndef _PWM_LIB_H
#define _PWM_LIB_H
//MAYBE NOT USEFUL
#include "Adafruit_PWMServoDriver.h"
#ifndef _ADAFRUIT_PWMServoDriver_H
#error Sketch must include Adafruit_PWMServoDriver.h
#endif

extern Adafruit_PWMServoDriver pwm;
extern int percent;
extern double pulseLength;
extern char serialString[100];
extern int MODE;
extern int prev_MODE;
//extern int x;

double mapVal(double val, double fromMin, double fromMax, double toMin, double toMax);
void sendPulse(int servoNum, double percent);
void setServoLow(int servoNum);
void setServoHigh(int servoNum);
void initializeMotor();

#endif 
