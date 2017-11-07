#include <MS5837.h>
#include <Wire.h>
#define HWSERIAL Serial1

int pressures [] = {0,0,0,0,0,0,0,0};
MS5837 sensor; // call pressure sensor "sensor"
float flexi;
float pressure;
String print1;
String print2;
String print3;
String print4;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  Wire.begin();
  sensor.setModel(MS5837::MS5837_30BA);
  sensor.setFluidDensity(997); // kg/m^3 (freshwater, 1029 for seawater)
}

void loop() {  
  //for (uint8_t i = 0; i < 8; i++) {
    flexi=analogRead(0);
    sensor.read(); // Update pressure and temperature readings
    //pressures[i]=sensor.pressure();
   pressure=sensor.altitude();
   print1=String(pressure,2);
   print2=String(flexi,2);
   print3=String(print1+ "," + print2+",");
   print4=String(print3+ "\n");
   Serial.println(pressure);
   Serial.println(print4);
   Serial1.print(print4);
  //}
  Serial.println("  ");
  delay(1000);
}

