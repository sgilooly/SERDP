int pot=23; //pot pin number
int therm = 22; //therm pin number
int greenLED = 16; //pin number
int blueLED = 15; //pin number
int redLED = 14; //pin number
int push1=8; //button 1 number
int push2=7; //button 2 number
int times=1000; //delay
float val1=800; //create variable
float val2; //create variable
float level=0.3; //tolerance
float c1;
float c2;
float f1;
float f2;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(38400); //open serial port
  pinMode(pot,INPUT);
  pinMode(therm,INPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(push1, INPUT);
  pinMode(push2, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  val2=analogRead(therm);
  c1 = 25 + (val1 - 512) / 11.3; //convert 1st to deg C
  f1 = c1 * 1.8 + 32; //convert 1st to deg F
  c2 = 25 + (val2 - 512) / 11.3; //convert 2nd to deg C
  f2 = c2 * 1.8 + 32; //convert 2nd to deg F
  
  if (f2 - f1 > level) {
     Serial.print("Heating Up to ");
     Serial.print(f2);
     Serial.print(" degrees F or ");
     Serial.print(c2);
     Serial.println(" degrees C");
     digitalWrite(redLED,HIGH);
     digitalWrite(greenLED,LOW);
     digitalWrite(blueLED,LOW);
  }
  else if (f2 - f1 < -1*level){
     Serial.print("Cooling Down to ");
     Serial.print(f2);
     Serial.print(" degrees F or ");
     Serial.print(c2);
     Serial.println(" degrees C");
     digitalWrite(redLED,LOW);
     digitalWrite(greenLED,LOW);
     digitalWrite(blueLED,HIGH);
    }
  else {
     Serial.print("Steady Temp at ");
     Serial.print(f2);
     Serial.print(" degrees F or ");
     Serial.print(c2);
     Serial.println(" degrees C");
     digitalWrite(redLED,LOW);
     digitalWrite(greenLED,HIGH);
     digitalWrite(blueLED,LOW);
    
    }
  delay(times);
  val1=val2;
}
