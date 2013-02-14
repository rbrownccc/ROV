//This code controls two motors
//motor A should be connected between A01 and A02
//motor B should be connected between B01 and B02

//The below code defines the output pins on the Arduino will hookup to specified pins on the HBridge
int STBY = 10; //this will be the standby pin
//Motor A
int PWMA = 3; //Speed control 
int AIN1 = 9; //Direction
int AIN2 = 8; //Direction
//Motor B
int PWMB = 5; //Speed control
int BIN1 = 11; //Direction
int BIN2 = 12; //Direction
int PS = 2;
int PSValue;
int normalPressure = 330;
int tooDeepTolerance = 10;
int tooShallowTolerance = 20;
int currentState = 0; //0 is stopped, 1 is forward, -1 is backwards

void setup(){
  pinMode(STBY, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  Serial.begin(9600);
  pinMode(PS, INPUT);
  currentState = 0;
}

void loop(){
  PSValue = analogRead(PS); 
  Serial.println(PSValue);
  delay(30);

if(PSValue > normalPressure - tooShallowTolerance && PSValue < normalPressure + tooDeepTolerance)//safe zone
{
  if (currentState != 0)
  {
    stop();
    currentState = 0;
  }
}
else if(PSValue < normalPressure - tooShallowTolerance)//too shallow
{
  if(currentState != 1)
  {
    move(1, 100, 1); //motor 1, full speed, left
    move(2, 100, 1); //motor 2, full speed, left
    currentState = 1;
  }
}
else if(PSValue > normalPressure - tooDeepTolerance)//too deep
{
  if (currentState != -1)
  {
    move(1, 100, 0);
    move(2, 100, 0);
    currentState = -1;
  }
}
}

void move(int motor, int speed, int direction){
//Move specific motor at speed and direction
//motor: 0 for B 1 for A
//speed: 0 is off, and 255 is full speed
//direction: 0 clockwise, 1 counter-clockwise

  digitalWrite(STBY, HIGH); //disable standby
  boolean inPin1 = LOW;
  boolean inPin2 = HIGH;
  if(direction == 1){
    inPin1 = HIGH;
    inPin2 = LOW;
  }
  if(motor == 1){
    digitalWrite(AIN1, inPin1);
    digitalWrite(AIN2, inPin2);
    analogWrite(PWMA, speed);
  }else{
    digitalWrite(BIN1, inPin1);
    digitalWrite(BIN2, inPin2);
    analogWrite(PWMB, speed);
  }
}
void stop(){
//enable standby  
  digitalWrite(STBY, LOW);
}

