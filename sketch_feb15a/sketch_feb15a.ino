#include <SPI.h>
#include <SD.h>
int chipSelect=10;
int num=0;
File SDTest;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(10,OUTPUT);
SD.begin(4);
}

void loop() {
  // put your main code here, to run repeatedly:
  num=num+1;
SDTest=SD.open("File.txt", FILE_WRITE);
if (SDTest) {
  Serial.print("Test #");
  Serial.print(num);
  Serial.println(" ");
  delay(250);

  SDTest.print("Test #");
  SDTest.print(num);
  SDTest.println("!");
  SDTest.close();
}
}

