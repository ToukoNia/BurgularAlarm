#include "device.h"

void Device::createDevice(int pinNo, bool currentStatus){
  pinNumber=pinNo;
  status=currentStatus;
}

bool Device::getStatus(){
  return status;
}


bool Sensor::readFromSensor(){
  return digitalRead(pinNumber);
}

void Sensor::updateStatus(bool update){
  status=update;
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
  digitalWrite(pinNumber,1);
}

void Buzzer::stop(){
  status=0;
  digitalWrite(pinNumber,0);
}

void LED::turnOn(){
  digitalWrite(pinNumber,1);
}

void LED::turnOff(){
  digitalWrite(pinNumber,0);
}