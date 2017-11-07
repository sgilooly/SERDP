/* SD Wiring: 
5V  --> 5V   GND --> GND  CLK --> 13   
DO  --> 12   DI  --> 11   CS  --> 10
*/
#include <SPI.h>
#include <SD.h>
int chipSelect=10;
float val1;
int times=100; //ms, delay
char filename[] = "D00.CSV";
File logfile;
void setup() {
Serial.begin(9600);
Serial.print("Initializing SD card...");
pinMode(10,OUTPUT);
  // see if the card is present and can be initialized:
 // initialize the SD card

  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    return;
  }
  Serial.println("card initialized.");
  
  // create a new file
 // char filename[] = "D00.CSV";
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
 
}

void loop() {
  // put your main code here, to run repeatedly:
  logfile = SD.open(filename, FILE_WRITE); 
  val1=analogRead(3);

  delay(times);
//SDPress=SD.open(filename, FILE_WRITE);
if (logfile) {
  Serial.print(val1);
  Serial.print(", ");
  
  logfile.println(val1);
  logfile.close();
  
}
}

