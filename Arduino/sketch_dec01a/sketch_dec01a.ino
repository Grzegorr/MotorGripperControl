void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
}

 

void loop() {
  int reading = 0;
  reading =  analogRead(A8);
  Serial.println(reading);
}
