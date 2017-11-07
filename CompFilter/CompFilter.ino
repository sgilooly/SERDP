
#include <Wire.h>
#include <SPI.h>
#include <SparkFunLSM9DS1.h>
LSM9DS1 imu;

#define LSM9DS1_M 0x1E // Would be 0x1C if SDO_M is LOW
#define LSM9DS1_AG  0x6B // Would be 0x6A if SDO_AG is LOW
#define PRINT_CALCULATED
//#define PRINT_RAW
#define PRINT_SPEED 100 // 250 ms between prints
static unsigned long lastPrint = 0; // Keep track of print time

// Earth's magnetic field varies by location. Add or subtract 
// a declination to get a more accurate heading. Calculate 
// your's here:
// http://www.ngdc.noaa.gov/geomag-web/#declination
#define DECLINATION 14.95 // Declination (degrees) in Durham, NH.
float roll;
float pitch;
float yaw;
  float gx_off;
  float gy_off;
  float gz_off;  
  float ax_off;
  float ay_off;
  float az_off;
  float roll_gyro;
  float pitch_gyro;
  float yaw_gyro;
  float roll_accel;
  float pitch_accel;

void setup() {
  
  Serial.begin(115200);
  
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
Serial.print("Mag Values: x: ");
Serial.print(m_x);Serial.print(" , y: ");Serial.print(m_y);Serial.print(" , z: ");Serial.println(m_z);

// Accel Angles ///////////////////////////////////////////////////////////
  float roll_accel = atan2(a_y, a_z);
  float pitch_accel = atan2(-a_x, sqrt(a_y * a_y + a_z * a_z));
  /*
  float heading;
  if (my == 0)
    heading = (mx < 0) ? PI : 0;
  else
    heading = atan2(mx, my);
    
  heading -= DECLINATION * PI / 180;
  
  if (heading > PI) heading -= (2 * PI);
  else if (heading < -PI) heading += (2 * PI);
  else if (heading < 0) heading += 2 * PI;
  */
  // Convert everything from radians to degrees:
  // heading *= 180.0 / PI;
  pitch_accel *= 180.0 / PI;
  roll_accel  *= 180.0 / PI;
  Serial.print("Accel Pitch, Roll: ");
  Serial.print(pitch_accel, 2);
  Serial.print(", ");
  Serial.println(roll_accel, 2);
  // Serial.print("Heading: "); Serial.println(heading, 2);
// Gyro Angles /////////////////////////////////////////////////////////////
float roll_gyro = roll+ g_x*PRINT_SPEED/1000;
float pitch_gyro = pitch+g_y*PRINT_SPEED/1000;
float yaw_gyro = yaw+g_z*PRINT_SPEED/1000;
Serial.print("Gyro Pitch, Roll: ");
  Serial.print(pitch_gyro, 2);
  Serial.print(", ");
  Serial.println(roll_gyro, 2);
  Serial.print("Yaw: "); Serial.println(yaw_gyro, 2);
   
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
  }

}








