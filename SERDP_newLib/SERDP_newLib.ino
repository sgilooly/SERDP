#include "Wire.h" // library for i2C communication
#include <MS5837.h> // library for pressure sensors

#define TCAADDR 0x70 //address for 1st multiplexer
#define TCAADDR1 0x71 //address for 2nd multiplexer
#define HWSERIAL Serial1 // define Serial1 for NGIMU connection
int lim=16;
int count=0;
long start;
String storage[]={" ", " ", " "," ", " "," "," "," ", " "," "," "," ", " "," "," "," "}; //string storing 16 pressure sensor values before sent to NGIMU
String printString; //string for stored pressure values with commas
MS5837 sensor[16];

void setup(){
  
  Wire.begin(); //initiate wire library
  Serial.begin(9600); //initiate serial port (teensy to computer, ger rid of all serial stuff later?}
  Serial1.begin(9600); //initiate serial1 port (teensy to NGIMU)
  Serial.println("Starting");
  //define pressure sensor parameters
  
  for (int i = 0; i < 8; i++){
    tcaselect1(i);
    sensor[i].init();
      delay(5);
    //  Serial.print("initializing: ");
    //  Serial.println(i);
    
    sensor[i].read();
    Serial.println(sensor[i].pressure());
    
    tcaselect(i);
    delay(250);
    sensor[i+8].init();
      delay(5);
    //  Serial.print("initializing: ");
    //  Serial.println( 8+ i);
    
    sensor[i+8].read();
    Serial.println(sensor[i + 8].pressure());   
   
  
  Serial.print("Setup Complete");
    
  } 
}
  


void loop(){
    count=count+1;
    start = micros();
    if (count <= 10) {
    for (int i = 0; i < lim; i++){
      multiplex(i);
      if ((sensor[i].pressure() > 2000) || (sensor[i].pressure() < 800)){
        sensor[i].init();
      }
    }
    }
    //time1=millis();
    for (int i = 0; i < lim; i++){
      multiplex(i);
      sensor[i].convert_P();
    }

    //delay(9);
    
    //for (int i = 0; i < lim; i++){
      //multiplex(i);
      //sensor[i].read_P();
    //}
    //time2=millis();
    for (int i = 0; i < lim; i++){
      multiplex(i);
      sensor[i].read_P();
      sensor[i].convert_T();
    }

    
    //delay(9);
    //for (int i = 0; i < lim; i++){
      //multiplex(i);
      //sensor[i].read_T();
    //}
    for (int i = 0; i < lim; i++){
      multiplex(i);
      sensor[i].read_T();
      sensor[i].calculate();
      storage[i]=String(sensor[i].pressure(),0);
    }
    for (int i = 0; i < lim; i++){
      Serial.print("Multiplexer ");
      Serial.print(i / 8);
      Serial.print(", sensor ");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(sensor[i].pressure());
    }
    Serial.print("Frequency ");
    Serial.println( (int)(micros() - start));
    printString = String(storage[0]+","+storage[1]+","+storage[2]+","+storage[3]+","+storage[4]+","+storage[5]+","+storage[6]+","+storage[7]+","+storage[8]+","+storage[9]+","+storage[10]+","+storage[11]+","+storage[12]+","+storage[13]+","+storage[14]+","+storage[15]+","+"\n");       
    Serial.println(printString);
    Serial1.println(printString);
    

} 
/*
void readAndWrite (uint8_t i) {
    sensor[i].read();
    Serial.print(sensor.pressure()); //print sensor value to computer
    Serial.print(" ");
    storage[i]=String(sensor.pressure(),0);
}*/



void tcaselect(uint8_t i) {
  if (i > 7) return;
  Wire.begin();
  Wire.beginTransmission(TCAADDR1);
  Wire.write(0);
  Wire.endTransmission();
  Wire.beginTransmission(TCAADDR);
  //Wire.write(0);
  Wire.write(1 << i);
  Wire.endTransmission();  
}

void tcaselect1(uint8_t i) {
  if (i > 7) return;
  Wire.beginTransmission(TCAADDR);
  Wire.write(0);
  Wire.endTransmission();
  Wire.beginTransmission(TCAADDR1);
  //Wire.write(0);
  Wire.write(1 << i);
  Wire.endTransmission();  
}

void multiplex(uint8_t i){
    if (i <= 7){
      tcaselect(i);
      }
     else{
        tcaselect1(i - 8);  
      }
  }


