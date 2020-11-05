const int encoder0PinA = 20; //Channel A
const int encoder0PinB = 21; //Channel B
const int enA = 4;  //PWM

unsigned long A_last_button_time = 0;
unsigned long A_button_time = 0;
int A_count = 0;


void setup() {
  pinMode(encoder0PinA, INPUT);
  pinMode(encoder0PinB, INPUT);
  attachInterrupt(digitalPinToInterrupt(encoder0PinA), A_Channel_Incrementor, FALLING);

  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Serial.println("Let's Go");
  }

void loop() {
  // put your main code here, to run repeatedly:

}

//Called by the Interrupt for A_Rising
void A_Channel_Incrementor(){
  Serial.println("Channel A triggered");
  A_button_time = micros();//millis() //millisecconds 
  
  //check to see if increment() was called in the last 100 microseconds 
  if (A_button_time - A_last_button_time > 10000)
  {
    A_count++;
    Serial.println("Ticks counted: " + String(A_count));
    A_last_button_time = A_button_time;
  }

   
}
