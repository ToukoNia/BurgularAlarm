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

void Lock::lock(){  //writes to a lock to tell it to lock
  status=1;
  digitalWrite(pinNumber,1);
}

void Lock::unlock(){  //writes to a lock to tell it to unlock
  status=0;
  digitalWrite(pinNumber,0);
}

void Buzzer::sound(){ //writes to a buzzer to tell it to make sound until stopped
  status=1;
  analogWrite(pinNumber,20);
}

void Buzzer::stop(){  //writes to a buzzer to tell it to stop sound
  status=0;
  analogWrite(pinNumber,0);
}

void LED::turnOn(){ //turns on an LED
  digitalWrite(pinNumber,1);
}

void LED::turnOff(){  //turns off an LED
  digitalWrite(pinNumber,0);
}

void LED::changeState(){  //changes state of LED
  if (status){
    turnOff();
  } else {
    turnOn();
  }
  status=!status;
}