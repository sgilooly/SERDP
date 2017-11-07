#include <SPI.h>
#include <SparkFunDS3234RTC.h>
#define DS13074_CS_PIN 10 // DeadOn RTC Chip-select pin
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  rtc.begin(DS13074_CS_PIN);
}

void loop() {
  
static int8_t lastSecond = -1;
  
  rtc.update();

  if (rtc.second() != lastSecond) // If the second has changed
  {
    printTime(); // Print the new time
    
    lastSecond = rtc.second(); // Update lastSecond value
  }
}
void printTime(){
// Read the time:
int s = rtc.second();
int m = rtc.minute();
int h = rtc.hour();

// Read the day/date:
int dy = rtc.day();
int da = rtc.date();
int mo = rtc.month();
int yr = rtc.year();
Serial.print(String(h) + ":"); // Print hour
  if (m < 10)
    Serial.print('0'); // Print leading '0' for minute
  Serial.print(String(m) + ":"); // Print minute
  if (s < 10)
    Serial.print('0'); // Print leading '0' for second
  Serial.print(String(s)); // Print second
  Serial.print(" - ");
Serial.print(String(mo) + "/" +   // Print month
                 String(da) + "/");  // Print date
Serial.println(String(rtc.year())+ " "); 

}
