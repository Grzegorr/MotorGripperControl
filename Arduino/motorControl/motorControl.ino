const byte enc0 = 20;
const byte enc0B =21;
volatile unsigned int encoder0Pos = 0;

void setup() {

  Serial.begin(9600);
  Serial.println("Hello!");
  //Setup Channel A
  pinMode(13, OUTPUT); //Initiates Motor Channel A pin
  pinMode(8, OUTPUT); //Initiates Brake Channel A pin

  pinMode(enc0, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(enc0), count0, CHANGE);
  //attachInterrupt(digitalPinToInterrupt(enc1), count1, CHANGE);
  
  
}

void loop(){
    motorf(255);
    delay(2000);
      
    stopm();
    delay(1000);
      
    motorb(127);
    delay(2000);
    
    stopm();
    delay(1000); 
}

/*
void loop(){

 //Close until sensor

float startpos = encoder0Pos;
float startread = reads;
while(reads()-startread >100){
  motorf(127);
}
 stopm();

// Find difference in encoder value to get radius 

for(int j =0;j<5;j++){
delay(500);
Serial.print(reads());
Serial.print(" ");

int estart = enc0Pos;

while(){
motorf();
}
stopm();
delay(100);
int estop = en0Pos;
int diff = estart - estop;
delay(500);
Serial.print(reads());
Serial.print(" ");
Serial.println(diff);
}

  
}

  //Return to side;
}
*/
void count0(){
  Serial.println("Interrupt triggered.");
 if (digitalRead(enc0) == HIGH) {  
    if(digitalRead(enc0B) == LOW){
      encoder0Pos = encoder0Pos + 1;
    }
    else
    {
      encoder0Pos = encoder0Pos - 1;
    }
}
 if(digitalRead(enc0) == LOW){
    if(digitalRead(enc0B) == LOW){
      encoder0Pos = encoder0Pos - 1;
    }
    else
    {
      encoder0Pos = encoder0Pos + 1;
    }
 }
 Serial.println(encoder0Pos,DEC);
}


void motorf( int mspeed){
  digitalWrite(13, HIGH); //Establishes forward direction of Channel A
  digitalWrite(8, LOW);   //Disengage the Brake for Channel A
  analogWrite(11, mspeed);   //Spins the motor on Channel A at full speed
}

void motorb( int mspeed){
  digitalWrite(13, LOW); //Establishes forward direction of Channel A
  digitalWrite(8, LOW);   //Disengage the Brake for Channel A
  analogWrite(11, mspeed);   //Spins the motor on Channel A at full speed
}

void stopm(){
  digitalWrite(8, HIGH);
  analogWrite(11, 0); 
}

float reads(){
float pread=0;
pread=0;
for(int i=0;i<10;i++){
  pread=pread+analogRead(A0);
}
pread=pread/10.0;
return pread;
}
