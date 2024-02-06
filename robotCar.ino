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
float numMeas = 10;
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
  int delayTime = 999;
  driveRight();
  if(motorAvg <= lowSpeed){
    Serial.println("Speed slow right turn");
    delay(delayTime);
  }
  if(motorAvg <= midSpeed){
    Serial.println("Speed mid right turn");
    delay(delayTime - 333);
  }
  if(motorAvg <= highSpeed){
    Serial.println("Speed mid right turn");
    delay(delayTime - 66);
  }
}


void ninetyDegreeTurnLeft(){
  int motorAvg = avgMotorSpeed();
  int delayTime = 250;
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
    Serial.println("Speed mid left turn");
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
 // Mode Switch input button on eleegoCar
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
  servoPos = 0;
  myServo.write(servoPos);
   delay(1000);
   
  readDistance();
  distanceScanOne = distanceInch;
 
  servoPos = 180;
  myServo.write(servoPos);
   delay(1000);
  readDistance();
  distanceScanTwo = distanceInch;
  delay(1000);
  servoPos = 90;
  myServo.write(servoPos);
   delay(1000);

  readDistance();
  distanceScanThree = distanceInch;

  Serial.print("Scan one: ");
  Serial.println(distanceScanOne);
  Serial.print("Scan two: ");
  Serial.println(distanceScanTwo);
  Serial.print("Scan three: ");
  Serial.println(distanceScanThree);

  if(distanceScanOne > distanceScanTwo && distanceScanThree){
    distanceScanWinner = 1;
     Serial.print("Scan Winner: ");
  Serial.println(distanceScanWinner);
    return distanceScanWinner;
} if (distanceScanThree > distanceScanOne && distanceScanTwo  ){
  distanceScanWinner = 3;
   Serial.print("Scan Winner: ");
  Serial.println(distanceScanWinner);
  return distanceScanWinner;
} if (distanceScanTwo > distanceScanThree && distanceScanOne){  distanceScanWinner = 2;
 Serial.print("Scan Winner: ");
  Serial.println(distanceScanWinner);
  return distanceScanWinner;}
   

}

void automaticTurns(){

int emptySpace = scanDistance();
driveBackward();
delay(1000);
if (emptySpace == 1){
 ninetyDegreeTurnLeft();
} 
if (emptySpace == 2 ){
ninetyDegreeTurnRight();
}
if (emptySpace == 3){
  driveForward();
  delay(2000);
}

}

void echoObstacleAvoid(bool switchState, unsigned long remoteID){
  while(switchState == true ){
  int loopCount = 0;
  cmd.value = 0;
  IR.resume();
  
while(IR.decode(&cmd)== 0){

   loopCount++;
   Serial.println(loopCount);
  delayMicroseconds(250);
  readDistance();
  driveForward();
Serial.println("Clear");

if(avgMotorSpeed() <= lowSpeed){
if(avgInchMeas < 12.0){
  wheelsOff();
  Serial.println("ObjectDetected");
automaticTurns();} 
}

if(avgMotorSpeed() >= midSpeed){
if(avgInchMeas < 15.0){
  wheelsOff();
  Serial.println("ObjectDetected");
automaticTurns();}
}

if(avgMotorSpeed() >= highSpeed){
if(avgInchMeas < 20.0){
  wheelsOff();
  Serial.println("ObjectDetected");
  automaticTurns();

} }
} 

Serial.print("Command value: ");
Serial.println(cmd.value);
Serial.println(avgMotorSpeed());

if(cmd.value == remoteID && loopCount > 1){
  switchState = false;
  Serial.print("ProgramStopped");
}
}

}


class remoteControl {

  private:

 


  public:

  int powerBtnState = false;
  int pauseBtnState = true;
  int funcStopState = false;
  unsigned long powerBtn = 0xFFA25D;
  unsigned long volumeUp = 0xFF629D;
  unsigned long funcStop = 0xFFE21D;
  unsigned long rewindBtn = 0xFF22DD;
  unsigned long pausePlay = 0xFF02FD;
  unsigned long fastForwardBtn= 0xFFC23D;
  unsigned long downBtn= 0xFFE01F;
  unsigned long volumeDown = 0xFFA857;
  unsigned long upBtn = 0xFF906F;
  unsigned long zeroBtn = 0xFF6897;
  unsigned long eqBtn = 0xFF9867;
  unsigned long stReptBtn = 0xFFB04F;
  unsigned long oneBtn = 0xFF30CF;
  unsigned long twoBtn = 0xFF18E7;
  unsigned long threeBtn = 0xFF7A85;
  unsigned long  fourBtn = 0xFF10EF;
  unsigned long fiveBtn = 0xFF38C7 ;
  unsigned long  sixBtn = 0xFF5AA5;
  unsigned long sevenBtn = 0xFF42BD;
  unsigned long eightBtn = 0xFF4AB5;
   unsigned long nineBtn = 0xFF52AD;



void sensorLoop(){
  while(IR.decode(&cmd)== 0){};
  delayMicroseconds(2500);
  IR.resume();
} 

  void printBtn(unsigned long signal){
    Serial.print("You clicked");
    Serial.println(signal,HEX);
  } 


void powerBtnCommand(unsigned long x){
  if(x == powerBtn) {
    powerBtnState = !powerBtnState;
 /*    digitalWrite(powerLedPin, powerBtnState ? HIGH : LOW); */

      if(powerBtnState == true){
        Serial.println("Power on");
        motorSwitchOn();
        
      } else {
        Serial.println("Power off");
        motorSwitchOff();
        wheelsOff();
         
      }
    }
 };
  

void volumeUpBtnCommand(unsigned long x){
  if(x == volumeUp){
    Serial.println("volume UP!!!");
    speedUp();
  }
}


void funcStopBtnCommand(unsigned long x){
  if(x == funcStop){
    obstacleDetect = !obstacleDetect;
    if(obstacleDetect == true){
      echoObstacleAvoid(obstacleDetect,funcStop);
    }
}
}


void rewindBtnCommand(unsigned long x){
  if(x == rewindBtn){
    driveLeft();
  }
}


void pausePlayBtnCommand(unsigned long x){
  if(x == pausePlay){
    pauseBtnState = !pauseBtnState;
    if(pauseBtnState == true){
    Serial.println("Pause");
    } else {
    Serial.println("Play");
    }
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


void volumeDownBtnCommand(unsigned long x){
  if(x == volumeDown){
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

void eqBtnCommand (unsigned long x){
  if(x == eqBtn){
  Serial.println("eq");
  }
  }


void stReptBtnCommand (unsigned long x){
  if(x == stReptBtn){
  changeMotorSpeedSwitch();

  }
  }


void oneBtnCommand (unsigned long x){
  if(x == oneBtn){
  Serial.println("one");
  }
  }

void twoBtnCommand(unsigned long x){
  if(x == twoBtn){
    Serial.println("Two");
  }
}

void threeBtnCommand(unsigned long x){
  if(x == threeBtn){
    Serial.println("Three");
  }
}


void fourBtnCommand(unsigned long x){
  if(x == fourBtn){
    Serial.println("four");
  }
}

void fiveBtnCommand(unsigned long x){
  if(x == fiveBtn){
    Serial.println("five");
  }
}

void sixBtnCommand(unsigned long x){
  if(x == sixBtn){
    Serial.println("six");
  }
}


void sevenBtnCommand(unsigned long x){
  if(x == sevenBtn){
    Serial.println("seven");
  }
}

void eightBtnCommand(unsigned long x){
  if(x == eightBtn){
    Serial.println("eight");
  }
}
void nineBtnCommand(unsigned long x){
  if(x == nineBtn){
    Serial.println("nine");
  }
}


void buttonCommands(unsigned long signal ){
 powerBtnCommand(signal);
 volumeUpBtnCommand(signal);
 funcStopBtnCommand(signal);
 rewindBtnCommand(signal);
 pausePlayBtnCommand(signal);
 fastForwardBtnCommand(signal);
 downBtnCommand(signal);
 volumeDownBtnCommand(signal);
 upBtnCommand(signal);
 zeroBtnCommand(signal);
 eqBtnCommand(signal);
 stReptBtnCommand(signal);
 oneBtnCommand(signal);
 twoBtnCommand(signal);
 threeBtnCommand(signal);
 fourBtnCommand(signal);
 fiveBtnCommand(signal);
 sixBtnCommand(signal);
 sevenBtnCommand(signal);
 eightBtnCommand(signal);
 nineBtnCommand(signal);
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


