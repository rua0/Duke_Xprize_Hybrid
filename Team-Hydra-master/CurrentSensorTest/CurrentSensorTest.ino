
// CURRENT SENSOR PIN
#define CURR_PIN  0

// SPECS OF CURRENT SENSOR
double maxV = 3.3; // volts
double minV = 0; // volts
double maxI = 100; // amps
double minI = 0; // amps

// INITIALIZE
double vReading = 0;
char currString[100];

// maps input value to output value for given input range and output range
double mapVal(double val, double fromMin, double fromMax, double toMin, double toMax){
  double out = 0;
  if (val < fromMin){ val = fromMin; }
  if (val > fromMax){ val = fromMax; }
  out = (((toMax-toMin)/(fromMax-fromMin))*(val-fromMin)) + toMin;
  return out; 
}

// map binary output of current sensor to current value
double mapCurr(int binValue){
  double vReading = mapVal(binValue,0,255,minV,maxV);
  double iReading = mapVal(vReading, minV, maxV, minI, maxI);
  
  return iReading;
}

void setup(){
  Serial.begin(9600);
  yield();
}

void loop(){
  // read current
  int binValue = analogRead(CURR_PIN);
  double iReading = mapCurr(binValue);

  // print current
  sprintf(currString,"current: %f", iReading);
  Serial.println(currString);

  delay(500);
}

