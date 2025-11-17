#include <IRremote.h>
#include <Servo.h>
#define irRemotePin 9
IRrecv IR(irRemotePin);
decode_results cmd;
#define servoPin 10
Servo myServo;
int servoPos;
  
#define echoTrigPin 13
#define echoPin 12
float distanceInch;
float echoReturnTime;
float measBucket = 0;
int j;
float numMeas = 5;
float avgInchMeas;
bool obstacleDetect = false;
int distanceScanWinner;

// define IO pin
#define motorRight 5   // Controls power to right motor
#define motorLeft 6    // Controls power to left motor
#define rightDirection 7     // Controls direction of right motor, HIGH = FORWARD, LOW = REVERSE
#define leftDirection 8     // Controls direction of left motor, HIGH = FORWARD, LOW = REVERSE
#define motorSwitch 3    // Place H-Bridge in standby if LOW, Run if HIGH
#define modeSwitch 2
#define lowSpeed 85
#define midSpeed 170
#define highSpeed 255

int rightSpeed = lowSpeed;
int leftSpeed = lowSpeed;
int speedSwitch = 0;
bool rightMotorOn;
bool leftMotorOn;



void leftMotorSpeed (){
analogWrite(motorLeft, leftSpeed);
leftMotorOn = true;
 
}

void fullMotorSpeed(){
  leftMotorSpeed();
  rightMotorSpeed();
}

void rightMotorOff(){
  digitalWrite(motorRight,LOW);
  rightMotorOn = false;
}
void leftMotorOff(){
  digitalWrite(motorLeft,LOW);
  leftMotorOn = false;

}

void driveLeft(){
  rightMotorSpeed();
  leftMotorOff();
}


void driveRight(){
  leftMotorSpeed();
  rightMotorOff();
}


void ninetyDegreeTurnRight(){
  int motorAvg = avgMotorSpeed();
  int delayTime = 150;
  driveRight();
  if(motorAvg <= lowSpeed){
    Serial.println("Speed slow right turn");
    delay(delayTime);
  }
  if(motorAvg <= midSpeed){
    Serial.println("Speed mid right turn");
    delay(delayTime * 2/3);
  }
  if(motorAvg <= highSpeed){
    Serial.println("Speed high right turn");
    delay(delayTime * 1/3);
  }
}


void ninetyDegreeTurnLeft(){
  int motorAvg = avgMotorSpeed();
  int delayTime = 150;
  driveLeft();
  if(motorAvg <= lowSpeed){
    Serial.println("Speed slow left turn");
    delay(delayTime);
  }
  if(motorAvg <= midSpeed){
    Serial.println("Speed mid left turn");
    delay(delayTime * 2/3);
  }
  if(motorAvg <= highSpeed){
    Serial.println("Speed high left turn");
    delay(delayTime * 1/3);
  }
}
 void driveForward(){
 // Drive car Forward for 1 second
  digitalWrite(rightDirection, HIGH);    // Forward direction on Right
  digitalWrite(leftDirection, HIGH);    // Forward direction on Left
  leftMotorSpeed();
  rightMotorSpeed();  
}
 
void wheelsOff(){   
  digitalWrite(motorRight, LOW); 
  digitalWrite(motorLeft, LOW); 
}
void motorSwitchOff(){
        digitalWrite(motorSwitch,LOW);
}
void motorSwitchOn(){
        digitalWrite(motorSwitch,HIGH);
}


void driveBackward(){  
  digitalWrite(rightDirection, LOW); 
  digitalWrite(leftDirection, LOW);
  rightMotorSpeed();
  leftMotorSpeed();
} 


void wheelCaseSpeedCheck(){
  if(rightMotorOn == true && leftMotorOn == true|| rightMotorOn == false && leftMotorOn == false){
fullMotorSpeed();}
if(rightMotorOn == true && leftMotorOn == false){
rightMotorSpeed();
} if(rightMotorOn == false && leftMotorOn == true){
  leftMotorSpeed();
}
}

void speedUp(){
  if(rightSpeed < 255){
    rightSpeed= rightSpeed +5;
    leftSpeed = leftSpeed +5;
  }
  wheelCaseSpeedCheck();
  Serial.print("Motor speed: ");
  Serial.println(rightSpeed);
}

void speedDown(){
  if(rightSpeed > 0){
      rightSpeed= rightSpeed -5;
    leftSpeed = leftSpeed -5;  
  }
   Serial.print("Motor speed: ");
  Serial.println(rightSpeed);
  wheelCaseSpeedCheck();

}



void changeMotorSpeedSwitch(){

speedSwitch = speedSwitch +1;
if (speedSwitch == 3){
  speedSwitch = 0;
}
 if(speedSwitch == 0){
  rightSpeed = lowSpeed;
  leftSpeed = lowSpeed;
}
if (speedSwitch == 1){
  rightSpeed = midSpeed;
  leftSpeed = midSpeed;
}
if (speedSwitch == 2){
   rightSpeed = highSpeed;
  leftSpeed = highSpeed;
} 

wheelCaseSpeedCheck();
Serial.print("Motor Speed");
Serial.println(rightSpeed);
Serial.print("speedSwitch Speed");
Serial.println(speedSwitch);
}




void rightMotorSpeed (){
analogWrite(motorRight,rightSpeed); 
leftMotorOn = false;

}

int avgMotorSpeed(){
int motorSpeedAverage = (leftSpeed + rightSpeed)/2;
return motorSpeedAverage;
}




void sendTriggerSignal(){
  digitalWrite(echoTrigPin,LOW);
  delayMicroseconds(10);
  digitalWrite(echoTrigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(echoTrigPin,LOW);
}

void readDistance(){
  measBucket = 0;
for(j =1; j<=numMeas; j++){
   sendTriggerSignal();
   echoReturnTime = pulseIn(echoPin,HIGH);
   distanceInch = (4.5/700.) * echoReturnTime + 1.25;
   measBucket = measBucket + distanceInch;
   }
    avgInchMeas = measBucket/numMeas;
Serial.println(avgInchMeas); }

int scanDistance(){
  int distanceScanOne;
  int distanceScanTwo;
  int distanceScanThree;
  servoPos = 90;
  myServo.write(servoPos);
  readDistance();
  distanceScanOne = avgInchMeas;
  servoPos = 0;
  myServo.write(servoPos);
  readDistance();
  distanceScanTwo = avgInchMeas;
  servoPos = 180;
  myServo.write(servoPos);
  readDistance();
  distanceScanThree = avgInchMeas;
  servoPos = 90;
  myServo.write(servoPos);



  Serial.print("Scan one: ");
  Serial.println(distanceScanOne);
  Serial.print("Scan two: ");
  Serial.println(distanceScanTwo);
  Serial.print("Scan three: ");
  Serial.println(distanceScanThree);

  if(distanceScanOne > distanceScanTwo && distanceScanThree)
  {
     distanceScanWinner = 1;
     Serial.print("Scan Winner: ");
     Serial.println(distanceScanWinner);
   
  } 

 

  if (distanceScanTwo > distanceScanThree && distanceScanOne)
  {  
    distanceScanWinner = 2;
    Serial.print("Scan Winner: ");
    Serial.println(distanceScanWinner);
   
  }

  if (distanceScanThree > distanceScanOne && distanceScanTwo  )
  {
     distanceScanWinner = 3;
     Serial.print("Scan Winner: ");
     Serial.println(distanceScanWinner);
    
  }
  return distanceScanWinner;
 

}

void automaticTurns(){

int emptySpace = scanDistance();
driveBackward();
delay(150);
digitalWrite(rightDirection, HIGH); 
digitalWrite(leftDirection, HIGH);
delay(50);
if (emptySpace == 1){
 driveForward();
} 
if (emptySpace == 2 ){
ninetyDegreeTurnRight();
}
if (emptySpace == 3){
   ninetyDegreeTurnLeft();
}

}

void echoObstacleAvoid(bool &switchState, unsigned long remoteID,unsigned long remoteID2){
   int loopCount = 0;
  while(switchState == true ){
  if (IR.decode(&cmd)) {
  IR.resume();  // Prepare for next signal
  delay(10);
  Serial.print("Command value: ");
  Serial.println(cmd.value);

  if (cmd.value == remoteID) {
    switchState = false;
    Serial.print("ProgramStopped");
  }

  if (cmd.value == remoteID2) {
    switchState = false;
    motorSwitchOff();
  }
 }

  
  loopCount++;
  Serial.println(loopCount);
  delayMicroseconds(250);
  readDistance();
  driveForward();
  Serial.println("Clear");

if(avgMotorSpeed() <= lowSpeed){
if(distanceInch < 14.0){
  wheelsOff();
  Serial.println("ObjectDetected");
automaticTurns();} 
}

if(avgMotorSpeed() >= midSpeed){
if(distanceInch < 17.0){
  wheelsOff();
  Serial.println("ObjectDetected");
automaticTurns();}
}

if(avgMotorSpeed() >= highSpeed){
if(distanceInch < 22.0){
  wheelsOff();
  Serial.println("ObjectDetected");
  automaticTurns();

} }



Serial.println(avgMotorSpeed());




} }


class remoteControl {

  private:

 


  public:
  bool obstacleDetect = false;
  int funcStopState = false;
  int okBtnState = false;
  unsigned long okBtn = 0xFF02FD;
  unsigned long oneBtn = 0xFF6897;
  unsigned long twoBtn = 0xFF9867;
  unsigned long starBtn = 0xFF42BD;
  unsigned long rewindBtn = 0xFF22DD;
  unsigned long fastForwardBtn= 0xFFC23D;
  unsigned long downBtn= 0xFFA857;
  unsigned long upBtn = 0xFF629D;
  unsigned long zeroBtn = 0xFF4AB5;
  unsigned long hashtagBtn = 0xFF52AD;
 


void sensorLoop(){
  while(IR.decode(&cmd)== 0){};
  delayMicroseconds(2500);
  IR.resume();
} 

  void printBtn(unsigned long signal){
    Serial.print("You clicked");
    Serial.println(signal,HEX);
  } 


void okBtnCommand(unsigned long x){
  if(x == okBtn) {
    okBtnState = !okBtnState;
 /*    digitalWrite(powerLedPin, okBtnState ? HIGH : LOW); */

      if(okBtnState == true){
        Serial.println("Power on");
        motorSwitchOn();
        
      } else {
        Serial.println("Power off");
        motorSwitchOff();
        wheelsOff();
         
      }
    }
 };
  

void twoBtnCommand(unsigned long x){
  if(x == twoBtn){
    Serial.println("volume UP!!!");
    speedUp();
  }
}


void starBtnCommand(unsigned long x){
  if(x == starBtn){
    obstacleDetect = !obstacleDetect;
    if(obstacleDetect == true){
      echoObstacleAvoid(obstacleDetect,starBtn,okBtn);
    }
}
}


void rewindBtnCommand(unsigned long x){
  if(x == rewindBtn){
    driveLeft();
  }
}


void fastForwardBtnCommand(unsigned long x){
  if(x == fastForwardBtn){
    driveRight();
  }
}

void downBtnCommand(unsigned long x){
  if(x == downBtn){
     driveBackward(); 

  }
}


void oneBtnCommand(unsigned long x){
  if(x == oneBtn){
    Serial.println("volume Down!!!");
    speedDown();
  }
}


void upBtnCommand(unsigned long x){
  if(x == upBtn){
    driveForward();
  }
}

void zeroBtnCommand (unsigned long x){
  if(x == zeroBtn){
   scanDistance();
  
 }
  }




void hashtagBtnCommand (unsigned long x){
  if(x == hashtagBtn){
  changeMotorSpeedSwitch();

  }
  }





void buttonCommands(unsigned long signal ){
 okBtnCommand(signal);
 oneBtnCommand(signal);
 twoBtnCommand(signal);
 starBtnCommand(signal);
 rewindBtnCommand(signal);
 fastForwardBtnCommand(signal);
 downBtnCommand(signal);
 upBtnCommand(signal);
 zeroBtnCommand(signal);
 hashtagBtnCommand(signal);
 Serial.println(signal,HEX);
} 
};


remoteControl remote;

  


//init the car
void setup() {
  pinMode(motorRight, OUTPUT);     //set IO pin mode OUTPUT
  pinMode(motorLeft, OUTPUT);
  pinMode(rightDirection, OUTPUT);
  pinMode(leftDirection, OUTPUT);
  pinMode(motorSwitch, OUTPUT);
  digitalWrite(motorSwitch, HIGH);  //Enable Motors to run
  IR.enableIRIn();
  Serial.begin(9600);
  myServo.attach(servoPin);
  pinMode(echoTrigPin,OUTPUT);
  pinMode(echoPin,INPUT);


}




void loop(){
remote.sensorLoop();
remote.buttonCommands(cmd.value);




 }


