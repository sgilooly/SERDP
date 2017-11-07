int num;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
}

void loop() {
  num=num+1;
  // put your main code here, to run repeatedly:
Serial.print("Test #");
//Serial.println(num);
delay(1000);
}
