int greenLED = 16; //pin number
int blueLED = 15; //pin number
int redLED = 14; //pin number
int times; //delay
int on=255; //intensity for on color
int off=0; //intensity for off color
String color; // string for color choice

void setup() {
  // put your setup code here, to run once:
  Serial.begin(38400); //open serial port
  pinMode(greenLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  pinMode(redLED, OUTPUT);

}

void loop() {
  delay(1000);
  Serial.print("What color would you like to see (red, green, blue)? ");
  while (Serial.available() == 0 ){
    }
  color=Serial.readString();
  Serial.println(color);

  Serial.print("And for how long (# seconds)? ");
  while (Serial.available() == 0 ){
    }
  times=Serial.parseInt();
  Serial.print(times);
  Serial.println(" s");
  times=1000*times; //convert time into ms from s

  if (color == "red") {
    analogWrite(greenLED, off);
    analogWrite(blueLED, off); 
    analogWrite(redLED, on); 
    delay(times); //wait 
    analogWrite(redLED, off);
    }
  if (color == "blue") {
    analogWrite(greenLED, off);
    analogWrite(blueLED, on); 
    analogWrite(redLED, off); 
    delay(times); //wait 
    analogWrite(blueLED, off);
    }
  if (color == "green") {
    analogWrite(greenLED, on);
    analogWrite(blueLED, off); 
    analogWrite(redLED, off); 
    delay(times); //wait 
    analogWrite(greenLED, off);
    }
Serial.println("  ");
}



