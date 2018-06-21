#include <LiquidCrystal.h>
// LCD Setup
LiquidCrystal lcd(12, 11, 5, 4, 3, 6);
// Tachometer Setup
volatile byte revolutions;
unsigned int rpm;
unsigned long timeold;

void setup() {
  // LCD Setup
  lcd.begin(20, 4);
  lcd.print("Time:");
  // Tachometer Setup
   Serial.begin(9600);
   attachInterrupt(0, magnet_detect, FALLING);//Initialize the intterrupt pin (Arduino digital pin 2)
   revolutions = 0;
   rpm = 0;
   timeold = 0;
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(5, 0);
  // print the number of seconds since reset:
  lcd.print(millis() / 1000);
  
  
  // Tochometer 
  lcd.setCursor(10,0);
  lcd.print("RPM:");
  if (revolutions >= 20) { 
     rpm = 60*1000/(millis() - timeold)*revolutions;
     timeold = millis();
     revolutions = 0;
     Serial.println(rpm,DEC);
     Serial.println(timeold);
      }
  lcd.setCursor(14,0);
  lcd.print(rpm);
 
  // Voltmeter
  lcd.setCursor(0,1);
  lcd.print("Voltage:");
  
  // Ammeter
  lcd.setCursor(0,2);
  lcd.print("Current:");
  
  // Multiplication Calculation
  lcd.setCursor(0,3);
  lcd.print("POOOOWAH:");
}

void magnet_detect()//This function is called whenever a magnet/interrupt is detected by the arduino
 {
   revolutions++;
   Serial.println(revolutions);
 }
