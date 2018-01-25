
/* SD Wiring: 
5V  --> 5V   GND --> GND  CLK --> 13   
DO  --> 12   DI  --> 11   CS  --> 10
FlexiForce: A1
Digikey: A4 and A5 (SDA and SCL)                                                    )


*/
#include <MS5837.h>
//#include <SPI.h>
//#include <SD.h>
#include <Wire.h>


MS5837 sensor; // call pressure sensor "sensor"
//int chipSelect=10; // pin used of MicroSD Card
int times=100; //ms, time delay
//char filename[] = "D00.CSV"; //begin with this name if no files exist
//int flexiPin=0; //analog pin connected to FlexiForce
//File logfile;   //create file handle   

void setup() {
Serial.begin(9600);
/*Serial.print("Initializing SD card...");
pinMode(chipSelect,OUTPUT);
  // initialize the SD card
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    return;
  }
  Serial.println("card initialized.");
  // create a new file
  for (uint8_t i = 0; i < 100; i++) {
    filename[1] = i/10 + '0';
    filename[2] = i%10 + '0';
    if (! SD.exists(filename)) {
      // only open a new file if it doesn't exist
      logfile = SD.open(filename, FILE_WRITE); 
      break;  // leave the loop!
    }
  }
   if (! logfile) {
    Serial.println("Error: couldnt create file");
  }
  Serial.print("Logging to: ");
  Serial.println(filename);
*/
  // Initialize pressure sensor
  Wire.begin();
  while (!sensor.init()) {
    Serial.println("Init failed!");
    Serial.println("Are SDA/SCL connected correctly?");
    Serial.println("Blue Robotics Bar30: White=SDA, Green=SCL");
    Serial.println("\n\n\n");
    delay(5000);
  }
  sensor.setModel(MS5837::MS5837_30BA);
  sensor.setFluidDensity(997); // kg/m^3 (freshwater, 1029 for seawater)
  
  //logfile=SD.open(filename, FILE_WRITE); 
  //logfile.println("Flexi, Pressure (mbar), Temp (deg C), Depth (m), Altitude (m above sea level)");
  //logfile.close();
  Serial.println("Pressure (mbar), Temp (deg C), Depth (m), Altitude (m above sea level)");
}

void loop() {
  //logfile = SD.open(filename, FILE_WRITE); 
  sensor.read(); // Update pressure and temperature readings
  
//if (logfile) {
  //Serial.print(analogRead(flexiPin));
  //Serial.print(", ");
  Serial.print(sensor.pressure()); 
  Serial.print(", ");
  Serial.print(sensor.temperature()); 
  Serial.print(", ");
  Serial.print(sensor.depth()); 
  Serial.print(", ");
  Serial.println(sensor.altitude()); 
  
  /*logfile.print(analogRead(flexiPin));
  logfile.print(", ");
  logfile.print(sensor.pressure()); 
  logfile.print(", ");
  logfile.print(sensor.temperature()); 
  logfile.print(", ");
  logfile.print(sensor.depth()); 
  logfile.print(", ");
  logfile.println(sensor.altitude()); 
  logfile.close();
  */
//}
delay(times);
}

   
