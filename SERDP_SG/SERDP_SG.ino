#include "Wire.h"
#include <MS5837.h>
//extern "C" {
//#include "utility/twi.h" // from Wire library, so we can do bus scanning
//}
#define TCAADDR 0x70
#define TCAADDR1 0x71
#define HWSERIAL Serial1
MS5837 sensor;

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



void setup(){
  Wire.begin();
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial.println("Setup Complete");
  Serial.println("  ");
  sensor.setModel(MS5837::MS5837_30BA);
  sensor.setFluidDensity(997); // kg/m^3 (freshwater, 1029 for seawater)

}

void loop(){
  Serial.println("Multiplexer 0:");
  for (uint8_t index=0; index<8; index++) {
    tcaselect(index);
    readAndWrite();
    delay(10);
  }
  Serial.println(" "); 
  
  Serial.println("Multiplexer 1:");
  for (uint8_t index=0; index<8; index++) {
    tcaselect1(index);
    readAndWrite();
    delay(10);
  }
  Serial.println(" "); 
  
}

void readAndWrite () {
if (!sensor.init()) {
  delay(10);
  if(!sensor.init()){
    Serial1.print("0.00");
    Serial.print(" miss ");
  }
}
else {
    sensor.read();
    Serial.print(sensor.pressure());
    Serial.print(" ");
  }
}  


