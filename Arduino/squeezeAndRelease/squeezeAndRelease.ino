// To use it, open serial monitor and type a distance in, inputs can be "2","10", "-33" etc. 


//Used Pins
const int encoder0PinA = 18; //Channel A of the encoder 
const int encoder0PinB = 19; //Channel B of the encoder
const int PWM_A = 11;  //PWM for signal control
const int DIR_1 = 8;  //Direction Control 
const int DIR_2 = 13;  //Direction control 
const int PressureSensor = 62;  // Analog input from pressure sensor

//For debouncing
unsigned long A_last_button_time = 0;
unsigned long A_button_time = 0;
int Debounce_time = 0; // Time between trips of encoder are allowed in microsecounds

int A_count = 0; //Position with Respect to srart
int saved_A = 6666;

int motor_pwm = 150; // How much power to move, 0-255 range

//int distances[] = {50, -50, 50 -50, 50, -50, 50 -50, 50, -50, 50 -50, 50, -50, 50 -50, 50, -50, 50 -50, 50, -50, 50 -50, 50, -50, 50 -50, 50, -50, 50 -50, 50, -50, 50 -50, 50, -50, 50 -50, 100, -100, 200, -200, 300, -300, 700, -700, 1500, -1500};
int distances[] = {30, -30, 30, -30, 30, -30, 30, -30, 30, -30, 30, -30, 30, -30, 30, -30, 30, -30, 30, -30, 30, -30, 30, -30, 30, -30, 30, -30, 30, -30, 30, -30, 30, -30, 30, -30, 30, -30, 30, -30,-6731,-6732};
int dummy = 0;

//Threshold when the inital squeez stops
//const int Sensor_thresh = 48;
int Sensor_thresh = 0;
int initial_squeeze_enc = 0;


//Squeezing action variables
int SA_initial_sensor = 0;
int SA_initial_A_count = 0;

int SA_sensor_threshold_delta = 5;//force to stop presqueeze
int SA_sensor_measurement_delta = 8;// max force squeeze
int SA_max_squeeze_distance = 500;

int SA_pre_squeeze_A = 0;


void setup() {
  pinMode(PWM_A, OUTPUT);
  pinMode(DIR_1, OUTPUT);
  pinMode(DIR_2, OUTPUT);
  Serial.begin(115200);
  Serial.println("Hello");
  attachInterrupt(digitalPinToInterrupt(encoder0PinA), A_Channel_Incrementor, FALLING); //Interrupt on A channel
  delay(300);
  analogWrite(PWM_A, motor_pwm);
  digitalWrite(DIR_1, LOW);
  digitalWrite(DIR_2, HIGH);
  Serial.println("Squeezing  now");
  delay(40000);
  Serial.println("End of squeezing");

  //6.Usqueeze to position(?) or completely open(?)
  int goal = 0;
  int distance = goal - A_count;
  if(distance > 0){
    //Go forward outputs
    analogWrite(PWM_A, motor_pwm);
    digitalWrite(DIR_1, LOW);
    digitalWrite(DIR_2, HIGH);
  }else{
    //Outputs to go back
    analogWrite(PWM_A, motor_pwm);
    digitalWrite(DIR_1, LOW);
    digitalWrite(DIR_2, LOW);
  }
  do{
    Serial.println("Needed for some reason");
    Serial.println("G:" + String(goal) + ", Current: " + String(A_count));
    if(abs(goal - A_count) < 60){
      analogWrite(PWM_A, 0.8*motor_pwm);
    }
    if(abs(goal - A_count) < 40){
      analogWrite(PWM_A, 0.5*motor_pwm);
    }
    if(abs(goal - A_count) < 20){
      analogWrite(PWM_A, 0.3*motor_pwm);
    }
  }while(A_count != goal);
  analogWrite(PWM_A, 0);



}












void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Looping");

}










//Called by the Interrupt for A_Rising
void A_Channel_Incrementor(){
  //Serial.println("Interrupt here");
  //delay(0);
  A_button_time = micros();//millis() //millisecconds 
  
  //check to see if increment() was called in the last 100 microseconds 
  if (A_button_time - A_last_button_time > Debounce_time)
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
