#include <Servo.h>          
#include <NewPing.h>  

int pos = 0;    
boolean fire = false;


const int LeftMotorForward = 4;
const int LeftMotorBackward =5;
const int RightMotorForward = 2;
const int RightMotorBackward = 3;

const int sensorMin = 0;     
const int sensorMax = 1024;  


#define trig_pin A0 
#define echo_pin A1 

#define Left_S A3   
#define Right_S A2     
#define Forward_S A4 
#define pump 6
 

#define maximum_distance 200
int distance = 100;

NewPing sonar(trig_pin, echo_pin, maximum_distance); 
Servo servo_motor; 
Servo myservo;

void setup(){
  Serial.begin(9600);
  pinMode(RightMotorForward, OUTPUT);
  pinMode(LeftMotorForward, OUTPUT);
  pinMode(LeftMotorBackward, OUTPUT);
  pinMode(RightMotorBackward, OUTPUT);
  
  pinMode(Left_S, INPUT);
  pinMode(Right_S, INPUT);
  pinMode(Forward_S, INPUT);
  myservo.attach(12);
  myservo.write(90); 
  
  servo_motor.attach(11); 
  servo_motor.write(115);
  delay(2000);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
}

void loop(){
  myservo.write(90);

  int range1 = analogRead(Left_S);
  int range2 = analogRead(Right_S);
  int range3 = analogRead(Forward_S);

  int distanceRight = 0;
  int distanceLeft = 0;
  delay(50);
  
  if (range1<=200||range2 <= 200 || range3 <=200){
    Serial.println("fire");
     if (range3 <=200){
      if(range3<65){
        Serial.println("firenear");
        fire=true;
      }
      else{
        Serial.println("firedistance");
        moveForward();
        delay(range3-8);
        
        fire=true;
      }
      }
     else if (range1 <=200){
      Serial.println("fire Left");
      right();
      delay(500);
      moveStop();
      delay(2000);
     }
     else if (range2 <=200) 
    {
      Serial.println("fire right");
      Serial.println(analogRead(Right_S));
      left();
      delay(500);
      moveStop();
      delay(2000);
    }
    
     while (fire == true)
     {
      put_off_fire();
     }
    
  }
    else if (distance <= 40){
      Serial.println("obstalcs");
      moveStop();
      delay(300);
      moveBackward();
      delay(400);
      moveStop();
      delay(300);
      distanceRight = lookRight();
      delay(300);
      distanceLeft = lookLeft();
      delay(300);
      if (analogRead(Forward_S) <=200 || analogRead(Right_S) <=200 || analogRead(Left_S) <=200) {
        return 0;
      }
      if (distanceRight >= distanceLeft){
        turnRight();
        moveStop();
      }
      else {
        turnLeft();
        moveStop();
      }
    
   }
    else{
    moveForward(); 
   }
    distance = readPing();
  
}

int lookRight(){  
  servo_motor.write(50);
  delay(1000);
  int distance = readPing();
  delay(1000);
  servo_motor.write(115);
  return distance;
  delay(100);
}

int lookLeft(){
  servo_motor.write(180);
  delay(1000);
  int distance = readPing();
  delay(1000);
  servo_motor.write(115);
  return distance;
  delay(100);
}

int readPing(){
  delay(70);
  int cm = sonar.ping_cm();
  if (cm==0){
    cm=250;
  }
  return cm;
}
void left(){
      digitalWrite(LeftMotorForward, HIGH);
      digitalWrite(LeftMotorBackward, LOW);
      digitalWrite(RightMotorForward, HIGH);
      digitalWrite(RightMotorBackward, HIGH);
}
void right(){
      digitalWrite(LeftMotorForward, HIGH);
      digitalWrite(LeftMotorBackward, HIGH);
      digitalWrite(RightMotorForward, HIGH);
      digitalWrite(RightMotorBackward, LOW);
}

void moveStop(){
  
  digitalWrite(RightMotorForward, LOW);
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(RightMotorBackward, LOW);
  digitalWrite(LeftMotorBackward, LOW);
}

void moveForward(){

    digitalWrite(LeftMotorForward, HIGH);
    digitalWrite(RightMotorForward, HIGH);
  
    digitalWrite(LeftMotorBackward, LOW);
    digitalWrite(RightMotorBackward, LOW); 
  
}

void moveBackward(){

  digitalWrite(LeftMotorBackward, HIGH);
  digitalWrite(RightMotorBackward, HIGH);
  
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(RightMotorForward, LOW);
  
}

void turnRight(){

  digitalWrite(LeftMotorForward, HIGH);
  digitalWrite(RightMotorBackward, HIGH);
  
  digitalWrite(LeftMotorBackward, LOW);
  digitalWrite(RightMotorForward, LOW);
  
  delay(500);
  
  digitalWrite(LeftMotorForward, HIGH);
  digitalWrite(RightMotorForward, HIGH);
  
  digitalWrite(LeftMotorBackward, LOW);
  digitalWrite(RightMotorBackward, LOW);
 
  
  
}

void turnLeft(){

  digitalWrite(LeftMotorBackward, HIGH);
  digitalWrite(RightMotorForward, HIGH);
  
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(RightMotorBackward, LOW);

  delay(500);
  
  digitalWrite(LeftMotorForward, HIGH);
  digitalWrite(RightMotorForward, HIGH);
  
  digitalWrite(LeftMotorBackward, LOW);
  digitalWrite(RightMotorBackward, LOW);
}
void put_off_fire()
{
  while(fire){
    /*if(analogRead(Forward_S)<10){
      moveBackward();
      delay(10);
    }*/
    
    
    
    delay (500);
    digitalWrite(LeftMotorForward, HIGH);
    digitalWrite(LeftMotorBackward, HIGH);
    digitalWrite(RightMotorForward, HIGH);
    digitalWrite(RightMotorBackward, HIGH);
    
   digitalWrite(pump, HIGH); 
   delay(500);
    
    for (pos = 50; pos <= 130; pos += 1) { 
    myservo.write(pos); 
    delay(10);  
  }
  for (pos = 130; pos >= 50; pos -= 1) { 
    myservo.write(pos); 
    delay(10);
  }
  
  digitalWrite(pump,LOW);
  myservo.write(90);
  if(analogRead(Forward_S)>60){
      fire=false;
    } 
}
}
 
