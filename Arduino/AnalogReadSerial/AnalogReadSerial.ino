/*
  AnalogReadSerial

  Reads an analog input on pin 0, prints the result to the Serial Monitor.
  Graphical representation is available using Serial Plotter (Tools > Serial Plotter menu).
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/AnalogReadSerial
*/

const int PressureSensor = 62;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0: 
  int value = 0; 
  int x = 0;
  for(x = 0; x <1; x++){
  int sensorValue = analogRead(PressureSensor);
  value = value + sensorValue;
  }
  Serial.println(value);
  delay(1);        // delay in between reads for stability
}
