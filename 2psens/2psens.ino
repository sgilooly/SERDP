#include <MS5837.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <SparkFun_MS5803_I2C.h>
#define TCAADDR 0x70

MS5803 sensor(ADDRESS_HIGH);
MS5837 sensor2; // call pressure sensor "sensor"
int chipSelect=10; // pin used of MicroSD Card
char filename[] = "D00.CSV"; //begin with this name if no files exist
File logfile;   //create file handle   
float temp_c;
double pressure_abs;
int times=75;

void tcaselect(uint8_t i) {
  if (i > 7) return;
  Wire.beginTransmission(TCAADDR);
  Wire.write(0);
  Wire.write(1 << i);
  Wire.endTransmission();  
}

void setup() {
    Serial.begin(9600);
    //Retrieve calibration constants for conversion math.
    sensor.reset();
    sensor.begin();

    Serial.print("Initializing SD card...");
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

  // Initialize pressure sensor
  Wire.begin();
  sensor2.setModel(MS5837::MS5837_30BA);
  sensor2.setFluidDensity(997); // kg/m^3 (freshwater, 1029 for seawater)
}

void loop() {
  logfile = SD.open(filename, FILE_WRITE); 
  if (logfile) {
  tcaselect(0);
  if (!sensor2.init()) {
  delay(5);
  if(!sensor2.init()){
    //Serial1.print("0.00");
    Serial.print(" miss ");
  }
}
else {
  sensor2.read();
  Serial.print(millis());
  Serial.print(", ");
  Serial.print(sensor2.pressure()); 
  Serial.print(", ");
  Serial.print(sensor2.temperature()); 

  logfile.print(millis());
  logfile.print(", ");
  logfile.print(sensor2.pressure()); 
  logfile.print(", ");
  logfile.print(sensor2.temperature()); 
  }
  
  tcaselect(1);
  sensor.reset();
  sensor.begin();
  
  pressure_abs = sensor.getPressure(ADC_4096);
  temp_c = sensor.getTemperature(CELSIUS, ADC_4096);
  Serial.print(", ");
  Serial.print(pressure_abs);
  Serial.print(", ");
  Serial.println(temp_c); 
  logfile.print(", ");
  logfile.print(pressure_abs); 
  logfile.print(", ");
  logfile.println(temp_c); 
}
  delay(times);
  logfile.close();
  }
  
  

