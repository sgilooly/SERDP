int greenLED = 16; //pin number
int blueLED = 15; //pin number
int redLED = 14; //pin number
int push1=8; //button 1 number
int push2=7; //button 2 number
int times=1000; //delay
int level;

void setup() {

  Serial.begin(38400); //open serial port
  pinMode(greenLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(push1, INPUT);
  pinMode(push2, INPUT_PULLUP);
}
void loop() {
  // put your main code here, to run repeatedly:

if (digitalRead(push1) == HIGH && digitalRead(push2) == HIGH) {
    Serial.println("No Button Pressed");
    digitalWrite(redLED,HIGH);
    digitalWrite(greenLED,LOW);
    digitalWrite(blueLED,LOW);
  } else if (digitalRead(push1) == LOW && digitalRead(push2) == LOW) {
    Serial.println("Both Buttons Pressed");
    digitalWrite(redLED,LOW);
    digitalWrite(greenLED,HIGH);
    digitalWrite(blueLED,LOW);
  } else {
      Serial.println("Only One Button Pressed");
    digitalWrite(redLED,LOW);
    digitalWrite(greenLED,LOW);
    digitalWrite(blueLED,HIGH);   
  }
  delay(250);
}
