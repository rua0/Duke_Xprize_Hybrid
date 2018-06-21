#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define CHOKE_IO   1
#define CHOKE_MIN  150
#define CHOKE_MAX  600

#define THRTL_IO   2
#define THRTL_MIN  150
#define THRTL_MAX  600

/*
 * Function for testing the range of a servo. It will go from the "from" position
 * to the "to" position, and then back to the "from" position.
 */
void fromTo(uint8_t servonum, uint16_t from, uint16_t to) {
  for (uint16_t pulselen = from; pulselen < to; pulselen++) {
    pwm.setPWM(servonum, 0, pulselen);
  }
  delay(500);
  for (uint16_t pulselen = to; pulselen > from; pulselen--) {
    pwm.setPWM(servonum, 0, pulselen);
  }
  delay(500);
}

void setup() {
  pwm.begin();
  pwm.setPWMFreq(60);
}

void loop() {
  fromTo(CHOKE_IO, CHOKE_MIN, CHOKE_MAX);
  fromTo(THRTL_IO, THRTL_MIN, THRTL_MAX);  
}
