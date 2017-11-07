int greenLED = 16; //pin number
int blueLED = 15; //pin number
int redLED = 14; //pin number
int times = 1000; //delay



void setup() {
  // put your setup code here, to run once:
  pinMode(greenLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  pinMode(redLED, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(blueLED,HIGH);
digitalWrite(redLED,LOW);
digitalWrite(greenLED,LOW);
delay(times);
digitalWrite(blueLED,LOW);
digitalWrite(redLED,HIGH);
digitalWrite(greenLED,LOW);
delay(times);
digitalWrite(blueLED,LOW);
digitalWrite(redLED,LOW);
digitalWrite(greenLED,HIGH);
delay(times);





}
