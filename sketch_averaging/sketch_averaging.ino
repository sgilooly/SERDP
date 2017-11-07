
int ar[5] = { 2, 4, 6, 8, 10};


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
float Num=0;
  for (int i = 0; i < 5; i++) {
        Serial.print(ar[i]);
        Serial.print(" ");
    Num=Num+ar[i];
    }
    Num=Num/5;
    Serial.println();
  Serial.print("The Average Is: ");
  Serial.print(Num);
  Serial.println(" !");
  // Num=ave.mean();
  delay(1000);
}
