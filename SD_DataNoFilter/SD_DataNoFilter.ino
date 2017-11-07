/* 
SD Wiring: 
5V  --> 5V   GND --> GND  CLK --> 13   
DO  --> 12   DI  --> 11   CS  --> 10
Accel Wiring
3.3V --> 3.3V GND --> GND
 SCL --> A5   SDA --> A4
*/

#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SparkFunLSM9DS1.h>
LSM9DS1 imu;

#define LSM9DS1_M 0x1E // Would be 0x1C if SDO_M is LOW
#define LSM9DS1_AG  0x6B // Would be 0x6A if SDO_AG is LOW
#define PRINT_CALCULATED
//#define PRINT_RAW
#define PRINT_SPEED 100 // ms between prints
static unsigned long lastPrint = 0; // Keep track of print time

// Earth's magnetic field varies by location. Add or subtract 
// a declination to get a more accurate heading. Calculate 
// your's here:
// http://www.ngdc.noaa.gov/geomag-web/#declination
#define DECLINATION 14.95 // Declination (degrees) in Durham, NH.

float gx_off;float gy_off;float gz_off;  
float ax_off;float ay_off;float az_off;
float Press1;

int chipSelect=10;
char filename[] = "D00.CSV";
File logfile;

void setup() {
  
  Serial.begin(115200);
  Serial.print(F("Initializing SD card..."));
pinMode(10,OUTPUT);
  // see if the card is present and can be initialized:
 // initialize the SD card

  while (!SD.begin(chipSelect)) {
    Serial.println(F("Card failed, or not present"));
  }
  Serial.println(F("card initialized."));
  //// START SD SETUP /////////////////////////////////////////
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
    Serial.println(F("couldn't create file"));
  }
  Serial.print(F("Logging to: "));
  Serial.println(filename);
  logfile.println(F("ax (g), ay (g), az (g), gx (dps), gy (dps), gz (dps), mx, my, mz, t (ms)"));
  logfile.close();
  ///// END SD SETUP //////////////////////////////////////////////////
  imu.settings.device.commInterface = IMU_MODE_I2C;
  imu.settings.device.mAddress = LSM9DS1_M;
  imu.settings.device.agAddress = LSM9DS1_AG;
  
  if (!imu.begin())
  {
    Serial.println(F("Failed to communicate with LSM9DS1."));
    while (1)
      ;
  }
////// CALCULATE OFFSETS //////////////////////////////////////////////
for (int i = 0; i < 5; i++) {
delay(100);
    imu.readGyro();
    imu.readAccel();
    imu.readMag();
    gx_off+=imu.gx;
    gy_off+=imu.gy;
    gz_off+=imu.gz;
  
    ax_off+=imu.ax;
    ay_off+=imu.ay;
    az_off+=imu.az-1;
}
    gx_off=gx_off/5;
    gy_off=gy_off/5;
    gz_off=gz_off/5;
  
    ax_off=ax_off/5;
    ay_off=ay_off/5;
    az_off=az_off/5;
}

void loop()
{

  // Update the sensor values whenever new data is available
  if ( imu.gyroAvailable() )
  {
    imu.readGyro();
  }
  if ( imu.accelAvailable() )
  {
    imu.readAccel();
  }
  if ( imu.magAvailable() )
  {
    imu.readMag();
  }
  
  if ((lastPrint + PRINT_SPEED) < millis())
  {
    Serial.println(F("Start Loop"));
    float a_x=imu.calcAccel(imu.ax)-ax_off;
    float a_y=imu.calcAccel(imu.ay)-ay_off;
    float a_z=imu.calcAccel(imu.az)-az_off;
    float g_x=imu.calcGyro(imu.gx)-gx_off;
    float g_y=imu.calcGyro(imu.gy)-gy_off;
    float g_z=imu.calcGyro(imu.gz)-gz_off;
    float m_x=imu.calcMag(imu.mx);
    float m_y=imu.calcMag(imu.my);
    float m_z=imu.calcMag(imu.mz);
    // pressure 
    Press1=analogRead(3);

    Serial.print(F("Printing Values for Time "));
    Serial.print(millis()); 
    Serial.println(F("msec"));
    
    logfile = SD.open(filename, FILE_WRITE); 
    
    logfile.print(a_x);logfile.print(", ");
    logfile.print(a_y);logfile.print(", ");
    logfile.print(a_z);logfile.print(", ");
    logfile.print(g_x);logfile.print(", ");
    logfile.print(g_y);logfile.print(", ");
    logfile.print(g_z);logfile.print(", ");
    logfile.print(m_x);logfile.print(", ");
    logfile.print(m_y);logfile.print(", ");
    logfile.print(m_z);logfile.print(", ");
    logfile.print(Press1);
    logfile.println(millis()); 
    
    logfile.close();
  }

}








