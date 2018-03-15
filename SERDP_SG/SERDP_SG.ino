#include "Wire.h"
#include <MS5837.h>

#define TCAADDR 0x70
#define TCAADDR1 0x71
#define HWSERIAL Serial1
MS5837 sensor;

float pressures [2][8] = {  {0,1,2,3,4,5,6,7},
                            {8,9,10,11,12,13,14,15} };
String storage[]={" ", " ", " "," ", " "," "," "," ", " "," "," "," ", " "," "," "," "};
String printString;
uint8_t val;


void setup(){
  Wire.begin();
  Serial.begin(9600);
  Serial1.begin(9600);
  sensor.setModel(MS5837::MS5837_30BA);
  sensor.setFluidDensity(997); // kg/m^3 (freshwater, 1029 for seawater)

}

void loop(){
  Serial.println("Multiplexer 0:");
  for (uint8_t index=0; index<8; index++) {
    tcaselect(index);
    readAndWrite(index);  
  }
  Serial.println(" "); 
  
  Serial.println("Multiplexer 1:");
  for (uint8_t index=0; index<8; index++) {
    tcaselect1(index);
    val = 8+index;
    readAndWrite(val);
  }
  Serial.println(" ");
  printString = String(storage[0]+","+storage[1]+","+storage[2]+","+storage[3]+","+storage[4]+","+storage[5]+","+storage[6]+","+storage[7]+","+storage[8]+","+storage[9]+","+storage[10]+","+storage[11]+","+storage[12]+","+storage[13]+","+storage[14]+","+storage[15]+","+"\n");       
  Serial.println(printString);
  Serial.println(" "); 
  Serial1.print(printString);
  
}

void readAndWrite (uint8_t i) {
if (!sensor.init()) {
  delay(5);
  if(!sensor.init()){
    //Serial1.print("0.00");
    Serial.print(" miss ");
    storage[i]="0.00";
  }
}
else {
    sensor.read();
    Serial.print(sensor.pressure());
    Serial.print(" ");
    storage[i]=String(sensor.pressure(),0);
  }
}



void tcaselect(uint8_t i) {
  if (i > 7) return;
  Wire.begin();
  Wire.beginTransmission(TCAADDR1);
  Wire.write(0);
  //Wire.write(1 << 7);
  Wire.endTransmission();
  Wire.beginTransmission(TCAADDR);
  Wire.write(0);
  Wire.write(1 << i);
  Wire.endTransmission();  
}

void tcaselect1(uint8_t i) {
  if (i > 7) return;
  Wire.begin();
  Wire.beginTransmission(TCAADDR);
  Wire.write(0);
  //Wire.write(1 << 7);
  Wire.endTransmission();
  Wire.beginTransmission(TCAADDR1);
  Wire.write(0);
  Wire.write(1 << i);
  Wire.endTransmission();  
}
