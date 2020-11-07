// To use it, open serial monitor and type a distance in, inputs can be "2","10", "-33" etc. 


const int encoder0PinA = 20; //Channel A of the encoder 
const int encoder0PinB = 21; //Channel B of the encoder
const int PWM_A = 2;  //PWM for signal control
const int DIR_1 = 22;  //Direction Control, they should be (1,0) or (0,1) for movement 
const int DIR_2 = 24;  //Direction control 

//For debouncing
unsigned long A_last_button_time = 0;
unsigned long A_button_time = 0;
int Debounce_time = 1000; // Time between trips of encoder are allowed in microsecounds

int A_count = 0; //Position with Respect to srart

int motor_pwm = 255; // How much power to move, 0-255 range


void setup() {
  pinMode(encoder0PinA, INPUT_PULLUP);
  pinMode(encoder0PinB, INPUT_PULLUP);
  pinMode(PWM_A, OUTPUT);
  pinMode(DIR_1, OUTPUT);
  pinMode(DIR_2, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(encoder0PinA), A_Channel_Incrementor, FALLING); //Interrupt on A channel

  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Serial.println("Let's Go");
  }

void loop() {
  // put your main code here, to run repeatedly:
  moveByNUmber();
}

void moveByNUmber(){
  Serial.println("Going to the goal");
  int distance = input();
  int goal = A_count + distance;
  if(distance > 0){
    //Go forward outputs
    Serial.println("Going forward");
    analogWrite(PWM_A, motor_pwm);
    digitalWrite(DIR_1, LOW);
    digitalWrite(DIR_2, HIGH);
  }else{
    //Outputs to go back
    analogWrite(PWM_A, motor_pwm);
    digitalWrite(DIR_1, HIGH);
    digitalWrite(DIR_2, LOW);
  }
  while(A_count != goal){
    Serial.println("Goal " + String(goal) + ", Current position: " + String(A_count));
  }
  //analogWrite(PWM_A, 0);
  Serial.println("At the goal");
  }

//Called by the Interrupt for A_Rising
void A_Channel_Incrementor(){
  Serial.println("Channel A triggered, Channel B currently is:" + String(digitalRead(encoder0PinB)));
  A_button_time = micros();//millis() //millisecconds 
  
  //check to see if increment() was called in the last 100 microseconds 
  if (A_button_time - A_last_button_time > Debounce_time)
  {

    //On falling edge of channel A, use the state of B to determine direction.
    if (digitalRead(encoder0PinB)) {
      A_count++;
    }
    else {
      A_count--;
    }
    Serial.println("Ticks counted: " + String(A_count));
    A_last_button_time = A_button_time;
  }   
}


//Taking input from srial
int input() {
  String inString = "";
  while(Serial.available() == 0) { }
  delay(100);
  while (Serial.available() > 0) {  
    int inChar = Serial.read();
    Serial.println("Read Char: " + String(inChar));
    inString += (char)inChar;

  }
  int output = inString.toInt();
  Serial.println("The output is:" + String(output));  
  return output;

  }
