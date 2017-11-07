int pot=23; //pot pin number
int greenLED = 16; //pin number
int blueLED = 15; //pin number
int redLED = 14; //pin number
int push1=8; //button 1 number
int push2=7; //button 2 number
int times=1000; //delay
int val; //create label

void setup() {
  // put your setup code here, to run once:
  Serial.begin(38400); //open serial port
  pinMode(pot,INPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(push1, INPUT);
  pinMode(push2, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  val = analogRead(pot);
  Serial.print("Potentiometer Reading is: ");
  Serial.print(val);
  if (val <= 341) {
    Serial.println(" so the Red LED is On!");
    digitalWrite(redLED,HIGH);
    digitalWrite(greenLED,LOW);
    digitalWrite(blueLED,LOW);
  } else if (val >= 862) {
    Serial.println(" so the Green LED is On");
    digitalWrite(redLED,LOW);
    digitalWrite(greenLED,HIGH);
    digitalWrite(blueLED,LOW);
  } else {
    Serial.println(" so the Blue LED is On");
    digitalWrite(redLED,LOW);
    digitalWrite(greenLED,LOW);
    digitalWrite(blueLED,HIGH);   
  }
  delay(250);
}


