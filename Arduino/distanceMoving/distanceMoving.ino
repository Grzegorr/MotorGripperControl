// To use it, open serial monitor and type a distance in, inputs can be "2","10", "-33" etc. 


const int encoder0PinA = 20; //Channel A of the encoder 
const int encoder0PinB = 21; //Channel B of the encoder
const int PWM_A =11;  //PWM for signal control
const int DIR_1 = 8;  //Direction Control, they should be (1,0) or (0,1) for movement 
const int DIR_2 = 13;  //Direction control 

//For debouncing
unsigned long A_last_button_time = 0;
unsigned long A_button_time = 0;
int Debounce_time = 0; // Time between trips of encoder are allowed in microsecounds

int A_count = 10; //Position with Respect to srart

int motor_pwm = 85; // How much power to move, 0-255 range

int distances[] = {50, -50, 50 -50, 50, -50, 50 -50, 50, -50, 50 -50, 50, -50, 50 -50, 50, -50, 50 -50, 50, -50, 50 -50, 50, -50, 50 -50, 50, -50, 50 -50, 50, -50, 50 -50, 50, -50, 50 -50, 100, -100, 200, -200, 300, -300, 700, -700, 1500, -1500};
int dummy = 0;


void setup() {
  //pinMode(encoder0PinA, INPUT_PULLUP);
  //pinMode(encoder0PinB, INPUT_PULLUP);
  pinMode(PWM_A, OUTPUT);
  pinMode(DIR_1, OUTPUT);
  pinMode(DIR_2, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(encoder0PinA), A_Channel_Incrementor, FALLING); //Interrupt on A channel

  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
  Serial.println("Let's Go");
  }

void loop() {
  // put your main code here, to run repeatedly:
  moveByNUmber();
}

void moveByNUmber(){
  Serial.println("Tpye distance to move:");

  ///If distance from keyboard
  //int distance = input();

  //if distance from array
  int distance = distances[dummy];
  dummy = dummy + 1;
  
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
    digitalWrite(DIR_1, LOW);
    digitalWrite(DIR_2, LOW);
  }
  while(A_count != goal){
    Serial.println("G:" + String(goal) + ", Current: " + String(A_count));
  }
  analogWrite(PWM_A, 0);
  Serial.println("At the goal");
  }

//Called by the Interrupt for A_Rising
void A_Channel_Incrementor(){
  //Serial.println("Interrupt here");
  //delay(0);
  A_button_time = micros();//millis() //millisecconds 
  
  //check to see if increment() was called in the last 100 microseconds 
  if (A_button_time - A_last_button_time > Debounce_time)
  {
    Serial.println("A: " + String(digitalRead(encoder0PinA)) + ", B:" + String(digitalRead(encoder0PinB)));
    //On falling edge of channel A, use the state of B to determine direction.
    if (digitalRead(encoder0PinB) == HIGH) {
        //Serial.println("#####");
        A_count = A_count + 1;
    }
    else{
      A_count = A_count - 1;
    }
    //Serial.println("Ticks counted: " + String(A_count));
    A_last_button_time = A_button_time;
  }   
}


//Taking input from srial
int input() {
  String inString = "";
  while(Serial.available() == 0) { }
  delay(100);
  while (Serial.available() > 0) {
    delay(100);  
    int inChar = Serial.read();
    Serial.println("Read Char: " + String(inChar));
    inString += (char)inChar;

  }
  int output = inString.toInt();
  Serial.println("The output is:" + String(output));  
  return output;

  }
