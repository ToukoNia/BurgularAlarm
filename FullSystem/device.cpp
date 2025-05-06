#include "device.h"

void Device::createDevice(int pinNo, bool currentStatus, int type,String ID){ //creates a device and initialises it with the given properties needed for operation
  pinNumber=pinNo;
  status=currentStatus;
  name=ID;
  pinmode=type;
  if (!type){
    pinMode(pinNumber, OUTPUT);
  } else if (type==1){
    pinMode(pinNumber, INPUT_PULLUP);
  } else { //switch to switchcase
    pinMode(pinNumber,INPUT);
  }
}


String Device::getName(){ //returns the device name, used for screen print outs
  return name;
}

void Device::printOut(){  //prints out the pin number of the device
  Serial.print(String(pinNumber));
}


bool Sensor::readFromSensor(){  //return if a sensor is active, with flipping the logic depending on if it is negatively or positively wired
  if (status==1){
    return !digitalRead(pinNumber);
  }
  return digitalRead(pinNumber);
}

void Sensor::printOut(){  //extends device's function to prinout additional information needed for saving the file
  Device::printOut();
  Serial.print(","+String(status)+","+String(pinmode));
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