#include "device.h"

void Device::createDevice(int pinNo, bool currentStatus, int type,String ID){
  pinNumber=pinNo;
  status=currentStatus;
  name=ID;
  if (!type){
    pinMode(pinNumber, OUTPUT);
  } else if (type==1){
    pinMode(pinNumber, INPUT_PULLUP);
  } else { //switch to switchcase
    pinMode(pinNumber,INPUT);
  }
}

bool Device::getStatus(){
  return status;
}

String Device::getName(){
  return name;
}

bool Sensor::readFromSensor(){
  if (status==1){
    return !digitalRead(pinNumber);
  }
  return digitalRead(pinNumber);
}

void Lock::lock(){
  status=1;
  digitalWrite(pinNumber,1);
}

void Lock::unlock(){
  status=0;
  digitalWrite(pinNumber,0);
}

void Buzzer::sound(){
  status=1;
  analogWrite(pinNumber,20);
}

void Buzzer::stop(){
  status=0;
  analogWrite(pinNumber,0);
}

void LED::turnOn(){
  digitalWrite(pinNumber,1);
}

void LED::turnOff(){
  digitalWrite(pinNumber,0);
}

void LED::changeState(){
  if (status){
    turnOff();
  } else {
    turnOn();
  }
  status=!status;
}