// To use it, open serial monitor and type a distance in, inputs can be "2","10", "-33" etc. 


//Used Pins
const int encoder0PinA = 20; //Channel A of the encoder 
const int encoder0PinB = 21; //Channel B of the encoder
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

int motor_pwm = 100; // How much power to move, 0-255 range

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

int SA_sensor_threshold_delta = 3;//force to stop presqueeze
int SA_sensor_measurement_delta = 8;// max force squeeze
int SA_max_squeeze_distance = 500;

int SA_pre_squeeze_A = 0;


void setup() {
  attachInterrupt(digitalPinToInterrupt(encoder0PinA), A_Channel_Incrementor, FALLING); //Interrupt on A channel
  delay(100);
  Sensor_thresh = analogRead(PressureSensor);
  Sensor_thresh = Sensor_thresh + 2;
  Serial.println("Sensor threshold: " + String(Sensor_thresh));
  //pinMode(encoder0PinA, INPUT_PULLUP);
  //pinMode(encoder0PinB, INPUT_PULLUP);
  pinMode(PWM_A, OUTPUT);
  pinMode(DIR_1, OUTPUT);
  pinMode(DIR_2, OUTPUT);
 
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
  Serial.println("Let's Go");
  }

void loop() {
  // put your main code here, to run repeatedly:
  squeeze_action();
}


//Plan:
//1.Measure an open gripper sensor value and position
//2.Squueze until the sensor detects a force
//3.Record and send to python the position and force from sensor
//4.Start squeezing -  stope when either force increases by a certain value(for hard objects) or the gripper moves in by a certain value(soft objects)
//5.Record and send to python the position and force from sensor
//6.Usqueeze to position(?) or completely open(?)
void squeeze_action(){

  if(SA_pre_squeeze_A == 0){
  //1.Measure an open gripper sensor value and position
  SA_initial_sensor = analogRead(PressureSensor);
  SA_initial_A_count = A_count;

  //2.Squueze until the sensor detects a force
  Serial.println("Starting Initial squeeze.");
  analogWrite(PWM_A, motor_pwm);
  digitalWrite(DIR_1, LOW);
  digitalWrite(DIR_2, HIGH);
  while(1){
    int sensorValue = analogRead(PressureSensor);
    Serial.println("Sensor during presqueeze: " + String(sensorValue));
    if(sensorValue > SA_initial_sensor + SA_sensor_threshold_delta){
        analogWrite(PWM_A, 0);
        Serial.println("END OF INITIAL SQUEZE" + String(sensorValue));
        Serial.println("END_OF_INITIAL_SQUEZE_POSITION " + String(A_count));
        delay(1000);
        break;}
    }
  }

  //3.Record and send to python the position and force from sensor
  int x = 0;
  for(x = 0; x <100; x++){
    int sensorValue = analogRead(PressureSensor);
  Serial.println("AAA" + String(sensorValue));
  }
  Serial.println("BBB" + String(A_count));
  SA_pre_squeeze_A = A_count;

  //4.Start squeezing -  stope when either force increases by a certain value(for hard objects) or the gripper moves in by a certain value(soft objects)
  Serial.println("Starting Measurement Squeeze.");
  analogWrite(PWM_A, motor_pwm);
  digitalWrite(DIR_1, LOW);
  digitalWrite(DIR_2, HIGH);
  while(1){
    int sensorValue = analogRead(PressureSensor);
    //Serial.println("Sensor during measurement squeeze: " + String(sensorValue));
    //Serial.println("Current position: " + String(A_count));
    //Serial.println("Furthest squeeze: " + String(SA_pre_squeeze_A + SA_max_squeeze_distance));
    if(sensorValue > SA_initial_sensor + SA_sensor_measurement_delta){
        analogWrite(PWM_A, 0);
        Serial.println("Max squeeze force reached");
        delay(1000);
        break;}
    if(A_count > SA_pre_squeeze_A + SA_max_squeeze_distance){
        analogWrite(PWM_A, 0);
        Serial.println("Max squeeze distance reached");
        delay(1000);
        break;}
    }

  //5.Record and send to python the position and force from sensor
  x = 0;
  for(x = 0; x <100; x++){
    int sensorValue = analogRead(PressureSensor);
  Serial.println("CCC" + String(sensorValue));
  }
  Serial.println("DDD" + String(A_count));

  //6.Usqueeze to position(?) or completely open(?)
  int goal = SA_pre_squeeze_A;
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
    //Serial.println("G:" + String(goal) + ", Current: " + String(A_count));
    if(abs(goal - A_count) < 40){
      analogWrite(PWM_A, 0.8*motor_pwm);
    }
    if(abs(goal - A_count) < 40){
      analogWrite(PWM_A, 0.7*motor_pwm);
    }
    if(abs(goal - A_count) < 40){
      analogWrite(PWM_A, 0.6*motor_pwm);
    }
  }while(A_count != goal);
  analogWrite(PWM_A, 0);

  

  
}


void initial_squeze2(){
    //Go forward outputs
    Serial.println("Starting Initial squeeze.");
    analogWrite(PWM_A, motor_pwm);
    digitalWrite(DIR_1, LOW);
    digitalWrite(DIR_2, HIGH);
    while(1){
      int sensorValue = analogRead(A1);
      Serial.println("SensorIS: " + String(sensorValue));
      if(sensorValue > 5000){
        analogWrite(PWM_A, 0);
        Serial.println("END OF INITIAL SQUEZE" + String(sensorValue));
        Serial.println("END_OF_INITIAL_SQUEZE_POSITION " + String(A_count));
        initial_squeeze_enc = A_count;
        delay(1000);
        break;}
    }
}


//To facilitate both soft and hard objects, there are two stopping conditions -  distance travelled and force induced for every squeeze
void squeeze_it(){
  //initial position measurement
  Serial.println(A_count);
  //Measure for before squeeze
  analogWrite(PWM_A, 0);
  delay(500);
  Serial.println(A_count);
  int x = 0;
  for(x = 0; x <100; x++){
    int sensorValue = analogRead(PressureSensor);
  Serial.println("AAA" + String(sensorValue));
  }
  Serial.println("BBB" + String(A_count));
  if(saved_A == 6666){
  saved_A = A_count;
  }
  
  //Squeezing part
  analogWrite(PWM_A, motor_pwm);
  digitalWrite(DIR_1, LOW);
  digitalWrite(DIR_2, HIGH);
  delay(2000);
  analogWrite(PWM_A, 0);
  delay(500);
  x = 0;
  for(x = 0; x <100; x++){
    int sensorValue = analogRead(PressureSensor);
    Serial.println("AAA" + String(sensorValue));
  }
  //Serial.println("BBB" + String(A_count));

  //Unsqueeze
  int goal = saved_A;
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
    //delay(10);
    Serial.println("Needed for some reason");
    //Serial.println("G:" + String(goal) + ", Current: " + String(A_count));
  }while(A_count != goal);
  analogWrite(PWM_A, 0);


    
  }

void moveByNUmber(){
  Serial.println("Distance to move:");

  ///If distance from keyboard
  //int distance = input();

  //if distance from array
  //Serial.println(dummy);
  int distance = distances[dummy];
  Serial.println("Distance " + String(distance));
  Serial.println("Dummy " + String(dummy));
  dummy = dummy + 1;

  if(distance == -6731){  
    distance = -A_count;
  }

  if(distance == -6732){
    Serial.println("End of the program.");
    dummy = 0;
    exit(0);
  }
  
  int goal = A_count + distance;
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
    //delay(10);
    Serial.println("G:" + String(goal) + ", Current: " + String(A_count));
  }while(A_count != goal);
  
  //Serial.println("After loop");
  analogWrite(PWM_A, 0);
  delay(500);
  int x = 0;
  for(x = 0; x <100; x++){
  int sensorValue = analogRead(PressureSensor);
  Serial.println("AAA" + String(sensorValue));
  }
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


void initial_squeze(){
    //Go forward outputs
    Serial.println("Starting Initial squeeze.");
    analogWrite(PWM_A, motor_pwm);
    digitalWrite(DIR_1, LOW);
    digitalWrite(DIR_2, HIGH);
    while(1){
      int sensorValue = analogRead(PressureSensor);
      Serial.println("Sensor: " + String(sensorValue));
      if(sensorValue > Sensor_thresh){
        analogWrite(PWM_A, 0);
        Serial.println("END OF INITIAL SQUEZE" + String(sensorValue));
        Serial.println("END_OF_INITIAL_SQUEZE_POSITION " + String(A_count));
        initial_squeeze_enc = A_count;
        delay(1000);
        break;}
    }
}

   
