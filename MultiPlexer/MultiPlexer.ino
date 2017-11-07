/**
 * TCA9548 I2CScanner.pde -- I2C bus scanner for Arduino
 *
 * Based on code c. 2009, Tod E. Kurt, http://todbot.com/blog/
 *
 */
#include <MS5837.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
extern "C" { 
#include "utility/twi.h"  // from Wire library, so we can do bus scanning
}

#define TCAADDR 0x70

void tcaselect(uint8_t i) {
  if (i > 7) return;
 
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}


void setup()
{
    Serial.begin(9600);
    Serial.println("\nTCAScanner ready!");
    while (!Serial);
    delay(1000);


    Wire.begin();
    
    for (uint8_t t=0; t<8; t++) {
      tcaselect(t);
      Serial.print("TCA Port #"); Serial.println(t);

      for (uint8_t addr = 0; addr<=127; addr++) {
        if (addr == TCAADDR) continue;
      
        uint8_t data;
        if (! twi_writeTo(addr, &data, 0, 1, 1)) {
           Serial.print("Found I2C 0x");  Serial.println(addr,HEX);
           //String name = String("sensor" + String(t));
           //MS5837 name;
           //Serial.print("Sensor Name Is: ");Serial.println(name);
        }
      }
    }
    Serial.println("\n Setup Done");
}

void loop() 
{
  tcaselect(2);
  MS5837 sensor2;
  sensor2.read();
  sensor2.setModel(MS5837::MS5837_30BA);
  sensor2.setFluidDensity(997); // kg/m^3 (freshwater, 1029 for seawater)
  

  Serial.print(sensor2.pressure()); 
  Serial.print(", ");
  Serial.print(sensor2.temperature()); 
  Serial.print(", ");
  Serial.print(sensor2.depth()); 
  Serial.print(", ");
  Serial.println(sensor2.altitude()); 
  
  delay(100);
}
