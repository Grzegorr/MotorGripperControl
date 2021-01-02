#include <Wire.h>
#include <Adafruit_ADS1015.h>

Adafruit_ADS1015 ads1015;

//Used Pins
const int encoder0PinA = 19; //Channel A of the encoder 
const int encoder0PinB = 18; //Channel B of the encoder
const int PWM_A = 11;  //PWM for signal control
const int DIR_1 = 8;  //Direction Control 
const int DIR_2 = 13;  //Direction control 

int A_count = 0;

//For debouncing
unsigned long A_last_button_time = 0;
unsigned long A_button_time = 0;
int Debounce_time = 0; // Time between trips of encoder are allowed in microsecounds



int motor_pwm = 180; // How much power to move, 0-255 range

void setup(void) {
  attachInterrupt(digitalPinToInterrupt(encoder0PinA), A_Channel_Incrementor, FALLING); //Interrupt on A channel
  ads1015.begin();
  delay(1000);
  //pinMode(encoder0PinA, INPUT_PULLUP);
  //pinMode(encoder0PinB, INPUT_PULLUP);
  pinMode(PWM_A, OUTPUT);
  pinMode(DIR_1, OUTPUT);
  pinMode(DIR_2, OUTPUT);
 
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
  //Serial.println("Let's Go");
  analogWrite(PWM_A, motor_pwm);
  digitalWrite(DIR_1, LOW);
  digitalWrite(DIR_2, HIGH);


}

void loop(void){
  int16_t adc0;
  int sensorSum = 0;
  
  int x = 0;
  for(x = 0; x <128; x++){
    adc0 = ads1015.readADC_SingleEnded(0);
    sensorSum = sensorSum + adc0;
  }
  Serial.println("Sensor Value: " + String(sensorSum/128.));
  Serial.println("A_count: " + String(A_count));

}














//Called by the Interrupt for A_Rising
void A_Channel_Incrementor(){
  //Serial.println("Interrupt here");
  //delay(0);
  A_button_time = micros();//millis() //millisecconds 
  
  //check to see if increment() was called in the last 100 microseconds 
  if (A_button_time - A_last_button_time >= Debounce_time)
  {
    //Serial.println("A: " + String(digitalRead(encoder0PinA)) + ", B:" + String(digitalRead(encoder0PinB)));
    //On falling edge of channel A, use the state of B to determine direction.
    if (digitalRead(encoder0PinB) == HIGH) {
        //Serial.println(A_count);
        A_count = A_count + 1;
    }
    else{
      A_count = A_count - 1;
    }
    //Serial.println("Ticks counted: " + String(A_count));
    A_last_button_time = A_button_time;
  } 
  //Serial.println(A_count);  
}
