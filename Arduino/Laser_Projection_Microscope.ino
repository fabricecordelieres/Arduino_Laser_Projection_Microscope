
#include <Nextion.h>

#include <NexButton.h>
#include <NexNumber.h>
#include <NexDualStateButton.h>

// Declare all the elements from the GUI

NexButton btnTiltDown=NexButton(0, 6, "btnTiltDown");
NexButton btnTiltUp=NexButton(0, 7, "btnTiltUp");
NexButton btnTurnDown=NexButton(0, 8, "btnTurnDown");
NexButton btnTurnUp=NexButton(0, 9, "btnTurnUp");

NexNumber tiltAngle=NexNumber(0, 4, "tiltAngle");
NexNumber turnAngle=NexNumber(0, 5, "turnAngle");

NexButton btnReset=NexButton(0, 3, "btnReset");
NexDSButton btnLaser=NexDSButton(0, 2, "btnLaser");


// Register a button object to the touch event list.  
NexTouch *nex_listen_list[] = {
  &btnTiltDown,
  &btnTiltUp,
  &btnTurnDown,
  &btnTurnUp,
  &btnReset,
  &btnLaser,
  NULL
};

#include <Servo.h>

// Declare stepper motors and the laser
Servo laserServo;
int laserServoPin=3;
Servo tableServo;
int tableServoPin=4;
int laserPin=2;

int tiltAngleValue=90;
int turnAngleValue=90;

void setup() {
  Serial.begin(9600);
  while (!Serial); // wait for serial port to connect. Needed for native USB

  nexInit();

  btnTiltDown.attachPop(btnTiltDownCallback, &btnTiltDown);
  btnTiltUp.attachPop(btnTiltUpCallback, &btnTiltUp);
  btnTurnDown.attachPop(btnTurnDownCallback, &btnTurnDown);
  btnTurnUp.attachPop(btnTurnUpCallback, &btnTurnUp);
  btnReset.attachPop(btnResetCallback, &btnReset);
  btnLaser.attachPop(btnLaserCallback, &btnLaser);

  pinMode(laserPin, OUTPUT);

  reset();
}

void loop() {
  nexLoop(nex_listen_list);
}

void reset(){
  servoMove(laserServo, laserServoPin, 90);
  servoMove(tableServo, tableServoPin, 90);
  
  digitalWrite(laserPin, LOW);

  tiltAngleValue=90;
  turnAngleValue=90;
  
  tiltAngle.setValue(tiltAngleValue);
  turnAngle.setValue(turnAngleValue);
  btnLaser.setValue(0);
}

void servoMove(Servo servo, int pin, int angle){
  servo.attach(pin);
  servo.write(angle);
  delay(100);
  servo.detach();
}

void btnTiltDownCallback(void *ptr){
  tiltAngleValue--;
  setTiltAngleValue();
}

void btnTiltUpCallback(void *ptr){
  tiltAngleValue++;
  setTiltAngleValue();
}

void setTiltAngleValue(){
  if(tiltAngleValue>135) tiltAngleValue=135;
  if(tiltAngleValue<45) tiltAngleValue=45;
  tiltAngle.setValue(tiltAngleValue);
  servoMove(laserServo, laserServoPin, tiltAngleValue);
}

void btnTurnDownCallback(void *ptr){
  turnAngleValue++;
  setTurnAngleValue();
}

void btnTurnUpCallback(void *ptr){
  turnAngleValue--;
  setTurnAngleValue();
}

void setTurnAngleValue(){
  if(turnAngleValue>135) turnAngleValue=135;
  if(turnAngleValue<45) turnAngleValue=45;
  turnAngle.setValue(turnAngleValue);
  servoMove(tableServo, tableServoPin, turnAngleValue);
}

void btnResetCallback(void *ptr){
  reset();
}

void btnLaserCallback(void *ptr){
  uint32_t laser_state;
  btnLaser.getValue(&laser_state);

  if(laser_state){
    digitalWrite(laserPin, HIGH);
  }else{
    digitalWrite(laserPin, LOW);
  }
}

