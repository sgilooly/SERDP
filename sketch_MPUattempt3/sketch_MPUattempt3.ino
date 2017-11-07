#include <Wire.h>

long  accelX, accelY, accelZ;
float gForceX, gForceY, gForceZ;
int   num=100;
long  gyroX, gyroY, gyroZ;
float rotX, rotY, rotZ;
long  mean_ax=0;
long  mean_ay=0;
long  mean_az=0;
long  mean_gx=0;
long  mean_gy=0;
long  mean_gz=0;  
  
long  ax_offset=0;
long  ay_offset=0;
long  az_offset=0;
long  gx_offset=0;
long  gy_offset=0;
long  gz_offset=0; 

long  accelXoff=0;
long  accelYoff=0;
long  accelZoff=0;
long  gyroXoff=0;
long  gyroYoff=0;
long  gyroZoff=0;



void setup() {
  Serial.begin(9600);
  Wire.begin();
  setupMPU();
  calibrate();
}


void loop() {
  recordAccelRegisters();
  recordGyroRegisters();
  printData();
  delay(100);
}

void setupMPU(){
  Wire.beginTransmission(0b1101000); //This is the I2C address of the MPU (b1101000/b1101001 for AC0 low/high datasheet sec. 9.2)
  Wire.write(0x6B); //Accessing the register 6B - Power Management (Sec. 4.28)
  Wire.write(0b00000000); //Setting SLEEP register to 0. (Required; see Note on p. 9)
  Wire.endTransmission();  
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x1B); //Accessing the register 1B - Gyroscope Configuration (Sec. 4.4) 
  Wire.write(0x00000000); //Setting the gyro to full scale +/- 250deg./s 
  Wire.endTransmission(); 
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x1C); //Accessing the register 1C - Acccelerometer Configuration (Sec. 4.5) 
  Wire.write(0b00000000); //Setting the accel to +/- 2g
  Wire.endTransmission(); 
}
void calibrate() {
  delay(1000);
   for (int i = 0; i < num; i++) {
    recordAccelRegisters();
    recordGyroRegisters();
    accelXoff=accelXoff+accelX;
    accelYoff=accelYoff+accelY;
    accelZoff=accelZoff+accelZ;
    gyroXoff=gyroXoff+gyroX;
    gyroYoff=gyroYoff+gyroY;
    gyroZoff=gyroZoff+gyroZ; 
    delay(2);
   }
   mean_ax=accelXoff/num;
   mean_ay=accelYoff/num;
   mean_az=accelZoff/num;
   mean_gx=gyroXoff/num;
   mean_gy=gyroYoff/num;
   mean_gz=gyroZoff/num;   
  
  ax_offset=-mean_ax;
  ay_offset=-mean_ay;
  az_offset=(16384-mean_az);

  gx_offset=-mean_gx;
  gy_offset=-mean_gy;
  gz_offset=-mean_gz;     

  recordAccelRegisters();
  recordGyroRegisters();
  Serial.println("-------------Test Calibration-------------");
  Serial.print("\nSensor readings with offsets:\t");
  Serial.print(accelX); 
  Serial.print("\t");
  Serial.print(accelY); 
  Serial.print("\t");
  Serial.print(accelZ); 
  Serial.print("\t");
  Serial.print(gyroX); 
  Serial.print("\t");
  Serial.print(gyroY); 
  Serial.print("\t");
  Serial.println(gyroZ);
  Serial.print("Your offsets:\t");
  Serial.print(ax_offset); 
  Serial.print("\t");
  Serial.print(ay_offset); 
  Serial.print("\t");
  Serial.print(az_offset); 
  Serial.print("\t");
  Serial.print(gx_offset); 
  Serial.print("\t");
  Serial.print(gy_offset); 
  Serial.print("\t");
  Serial.println(gz_offset); 
  Serial.println("\nData is printed as: acelX acelY acelZ giroX giroY giroZ");
  Serial.println("Check that your sensor readings are close to 0 0 16384 0 0 0");
  Serial.println("If calibration was succesful write down your offsets so you can set them in your projects using something similar to mpu.setXAccelOffset(youroffset)");
  Serial.println("-------------Test Calibration-------------");
  Serial.println();
}


void recordAccelRegisters() {
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x3B); //Starting register for Accel Readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6); //Request Accel Registers (3B - 40)
  while(Wire.available() < 6);
  accelX = Wire.read()<<8|Wire.read(); //Store first two bytes into accelX
  accelY = Wire.read()<<8|Wire.read(); //Store middle two bytes into accelY
  accelZ = Wire.read()<<8|Wire.read(); //Store last two bytes into accelZ
  accelX=accelX+ax_offset;
  accelY=accelY+ay_offset;
  accelZ=accelZ+az_offset;
  gForceX = accelX / 16384.0;
  gForceY = accelY / 16384.0; 
  gForceZ = accelZ / 16384.0;


}


void recordGyroRegisters() {
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x43); //Starting register for Gyro Readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6); //Request Gyro Registers (43 - 48)
  while(Wire.available() < 6);
  gyroX = Wire.read()<<8|Wire.read(); //Store first two bytes into accelX
  gyroY = Wire.read()<<8|Wire.read(); //Store middle two bytes into accelY
  gyroZ = Wire.read()<<8|Wire.read(); //Store last two bytes into accelZ
  gyroX=gyroX+gx_offset;
  gyroY=gyroY+gy_offset;
  gyroZ=gyroZ+gz_offset;
  rotX = gyroX / 131.0;
  rotY = gyroY / 131.0; 
  rotZ = gyroZ / 131.0;
  
}

void printData() {
  Serial.print("Gyro (deg)");
  Serial.print(" X=");
  Serial.print(rotX);
  Serial.print(" Y=");
  Serial.print(rotY);
  Serial.print(" Z=");
  Serial.print(rotZ);
  Serial.print(" Accel (g)");
  Serial.print(" X=");
  Serial.print(gForceX);
  Serial.print(" Y=");
  Serial.print(gForceY);
  Serial.print(" Z=");
  Serial.println(gForceZ);
}




