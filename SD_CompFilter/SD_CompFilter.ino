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

float roll;float pitch;float yaw;
float gx_off;float gy_off;float gz_off;  
float ax_off;float ay_off;float az_off;
float roll_gyro;float pitch_gyro;float yaw_gyro;
float roll_accel;float pitch_accel;

int chipSelect=10;
char filename[] = "D00.CSV";
File logfile;

void setup() {
  
  Serial.begin(115200);
  Serial.print("Initializing SD card...");
pinMode(10,OUTPUT);
  // see if the card is present and can be initialized:
 // initialize the SD card

  while (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
  }
  Serial.println("card initialized.");
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
    Serial.println("couldnt create file");
  }
  Serial.print("Logging to: ");
  Serial.println(filename);
  ///// END SD SETUP //////////////////////////////////////////////////
  imu.settings.device.commInterface = IMU_MODE_I2C;
  imu.settings.device.mAddress = LSM9DS1_M;
  imu.settings.device.agAddress = LSM9DS1_AG;
  
  if (!imu.begin())
  {
    Serial.println("Failed to communicate with LSM9DS1.");
    Serial.println("Double-check wiring.");
    Serial.println("Default settings in this sketch will " \
                  "work for an out of the box LSM9DS1 " \
                  "Breakout, but may need to be modified " \
                  "if the board jumpers are.");
    while (1)
      ;
  }
// try to calculate offsets
delay(100);
    imu.readGyro();
    imu.readAccel();
    imu.readMag();
  float gx_off=imu.gx;
  float gy_off=imu.gy;
  float gz_off=imu.gz;
  
  float ax_off=imu.ax;
  float ay_off=imu.ay;
  float az_off=imu.az-1;
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
    Serial.println("Start Loop");
    float a_x=imu.calcAccel(imu.ax)-ax_off;
    float a_y=imu.calcAccel(imu.ay)-ay_off;
    float a_z=imu.calcAccel(imu.az)-az_off;
    float g_x=imu.calcGyro(imu.gx)-gx_off;
    float g_y=imu.calcGyro(imu.gy)-gy_off;
    float g_z=imu.calcGyro(imu.gz)-gz_off;
    float m_x=imu.calcMag(imu.mx);
    float m_y=imu.calcMag(imu.my);
    float m_z=imu.calcMag(imu.mz);

//Serial.print("Gyro with offsets: x: ");
//Serial.print(g_x);Serial.print(" , y: ");Serial.print(g_y);Serial.print(" , z: ");Serial.println(g_z);
//Serial.print("Mag Values: x: ");
//Serial.print(m_x);Serial.print(" , y: ");Serial.print(m_y);Serial.print(" , z: ");Serial.println(m_z);

// Accel Angles ///////////////////////////////////////////////////////////
  float roll_accel = atan2(a_y, a_z);
  float pitch_accel = atan2(-a_x, sqrt(a_y * a_y + a_z * a_z));
  
  float heading;
  if (m_y == 0)
    heading = (m_x < 0) ? PI : 0;
  else
    heading = atan2(m_x, m_y);
    
  heading -= DECLINATION * PI / 180;
  
  if (heading > PI) heading -= (2 * PI);
  else if (heading < -PI) heading += (2 * PI);
  else if (heading < 0) heading += 2 * PI;
  
  // Convert everything from radians to degrees:
   heading *= 180.0 / PI;
  pitch_accel *= 180.0 / PI;
  roll_accel  *= 180.0 / PI;
  //Serial.print("Accel Pitch, Roll: ");
  //Serial.print(pitch_accel, 2);
  //Serial.print(", ");
  //Serial.println(roll_accel, 2);
  // Serial.print("Heading: "); Serial.println(heading, 2);
// Gyro Angles /////////////////////////////////////////////////////////////
float roll_gyro = roll+ g_x*PRINT_SPEED/1000;
float pitch_gyro = pitch+g_y*PRINT_SPEED/1000;
float yaw_gyro = yaw+g_z*PRINT_SPEED/1000;
//Serial.print("Gyro Pitch, Roll: ");
  //Serial.print(pitch_gyro, 2);
  //Serial.print(", ");
  //Serial.println(roll_gyro, 2);
  //Serial.print("Yaw: "); Serial.println(yaw_gyro, 2);
   
    lastPrint = millis(); // Update lastPrint time
    
// Complimentary Filter ////////////////////////////////////////////////////
float alpha= 0.8;
roll=alpha*roll_gyro+(1-alpha)*roll_accel;
pitch=alpha*pitch_gyro+(1-alpha)*pitch_accel;
yaw=yaw_gyro;
Serial.print("Comp Pitch, Roll: ");
  Serial.print(pitch, 2);
  Serial.print(", ");
  Serial.println(roll, 2);
  Serial.print("Yaw: "); Serial.println(yaw, 2);
  Serial.print("Alpha is :");
  Serial.println(alpha);
    Serial.println(); 
  logfile = SD.open(filename, FILE_WRITE); 
  // put in void setup?? logfile.println("Pitch, Roll, Yaw");
  logfile.print(pitch);
  logfile.print(", ");
  logfile.print(roll);
  logfile.print(", ");
  logfile.println(yaw);
  logfile.close();
  }

}








