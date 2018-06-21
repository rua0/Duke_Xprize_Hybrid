#include <Wire.h>
#include <Servo.h>
#include <string.h>

// SET PIN
#define SERVO_PIN 6
Servo myServo;


// DEFINE MIN/MAX THROTTLE DECIMAL PERCENTAGES TO INPUT
// in general:
    // 0 = 0%  no movement
    // 100 = 100% = full speed forwards
    // -100 = -100% = full speed backwards
const int minThrottle = -100;
const int maxThrottle = 100;


// DEFINE SERIAL INPUT
int percentThrottle = 0;
double pulseLength = 1500;
char serialString[100];

// sweep pulse lengths
void sweep(){
   Serial.println("Initializing...about to sweep");
  // sweep over pwm lengths from 1 ms to 2 ms and read the angle of the servo
  for (int i = 1000; i <= 2000; i++) { 
    myServo.writeMicroseconds(i);
    sprintf(serialString,"sweeping...pulseLength: %d \t angle: %d", i, myServo.read());
    Serial.println(serialString);
  }

}



void setup() {
  // setup esc servo
  myServo.attach(SERVO_PIN);
  Serial.begin(9600);

  pulseLength = 1500;
  
  yield();
}


// maps input value to output value for given input range and output range
int mapVal(int val, int fromMin, int fromMax, int toMin, int toMax){
  int out = 0;
  if (val < fromMin){ val = fromMin; }
  if (val > fromMax){ val = fromMax; }
  out = (((toMax-toMin)/(fromMax-fromMin))*(val-fromMin)) + toMin;
  return out; 
}


// maps throttle percentage to PWM pulse length in μs (converts ms ->μs!!!)
int convertToPL(int percentThrottle){
  int pulseLength = mapVal(percentThrottle,minThrottle,maxThrottle,minPL*pow(10,3),maxPL*pow(10,3));
  return pulseLength;
}

void sendPulse(int percentThrottle){
  double pulseLength = convertToPL(percentThrottle);
  myServo.writeMicroseconds(pulseLength);
}

void loop() {
  if (Serial.available()>0){
      pulseLength = Serial.parseFloat();
  }
  // print out 1000 * pulse length since serial monitor can't print floats in sprintf
  sprintf(serialString,"1000 * pulseLength: %d \t angle: %d", (int) (100*pulseLength), myServo.read());
  Serial.println(serialString);

  // convert throttle decimal percentage into pulse length duration and send pulse
  myServo.writeMicroseconds(pulseLength);
  
  delay(500);

}
