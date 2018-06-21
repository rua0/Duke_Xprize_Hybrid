#ifndef MY_GLOABAL_H
#define MY_GLOABAL_H
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include "Adafruit_PWMServoDriver.h"
#ifndef _ADAFRUIT_PWMServoDriver_H
#error Sketch must include SoftwareSerial.h
#endif

//pwm object
//volatile Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
//int x=10;
//SET PINS FOR SERVOS AND ESC
#define CHOKE_PIN  6
#define THROTTLE_PIN  5
#define ESC_PIN  3 

// DEFINE PWM FREQ IN Hz
#define FREQ 50
//yao: freq really low
// DEFINE NUMBER OF TICKS PER PULSE
#define NUM_TICKS  4096
const double TICK_LENGTH = pow(10,3)/(1.0*FREQ * NUM_TICKS); // gives ms/tick

// DEFINE MIN/MAX PWM LENGTHS IN MILLISECONDS (ms)
const double CHOKE_MIN_PL = 0.650; // closed choke, min air
const double CHOKE_MAX_PL = 1.200; // open choke, max air
 
const double THROTTLE_MIN_PL = 1.250; // closed (originally open/full throttle, max gas)
const double THROTTLE_MAX_PL = 1.925; // open   (closed/min throttle, min gas)

const double ESC_MIN_PL = 1;
const double ESC_MAX_PL = 2;


// DEFINE MIN/MAX THROTTLE DECIMAL PERCENTAGES TO INPUT
// in general:
    // 0 = 0%  no movement
    // 100 = 100% = full speed forwards
    // -100 = -100% = full speed backwards -- SPARK, THROTTLE, AND CHOKE SERVOS DON'T MOVE IN REVERSE DIRECTION!!!
const double CHOKE_MIN = 0;
const double CHOKE_MAX = 100;

const double THROTTLE_MIN = 0;
const double THROTTLE_MAX = 100;

const double ESC_MIN = 0;
const double ESC_MAX = 100;


// DEFINE SERIAL INPUT


//volatile int percent = 0;
//volatile double pulseLength = 1500;
//volatile char serialString[100];
//volatile int MODE = 0;



#endif 
