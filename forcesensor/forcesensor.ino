float val1;
float val2;
int times=100; //ms, delay


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //open serial port
  // pinMode(reading,INPUT);
  Serial.print(analogRead(1));
}
  
void loop() {
  // put your main code here, to run repeatedly:
  val1 = analogRead(1);
 // val2 = analogRead(2);
//  Serial.print("Output Reading is: ");
//  Serial.print(val1);
  //Serial.print(" and ");
  //Serial.print("Source Reading is: ");
  //Serial.println(val2);
  //Serial.println(" ");
  //Serial.print(", ");  
  Serial.println(val1);

  delay(times);
}


