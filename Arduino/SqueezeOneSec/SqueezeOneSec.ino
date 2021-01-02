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

int motor_pwm = 120; // How much power to move, 0-255 range



void setup() {
  pinMode(PWM_A, OUTPUT);
  pinMode(DIR_1, OUTPUT);
  pinMode(DIR_2, OUTPUT);
 
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
  Serial.println("Let's Go");
  analogWrite(PWM_A, motor_pwm);
  digitalWrite(DIR_1, LOW);
  digitalWrite(DIR_2, HIGH);

  delay(5000);

  analogWrite(PWM_A, 0);

}

void loop() {
  Serial.println("Loop for ever");

}
