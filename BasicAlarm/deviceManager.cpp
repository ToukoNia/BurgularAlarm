#include "deviceManager.h"

void SensorManager::addSensor(int pinNo){
  sensors[head]= Sensor();
  sensors[head].createDevice(pinNo,0); //change to constructors at somepoint
  head++;
}

void SensorManager::removeSensor(int number){
  tail=number;
  for (i=number+1;i<head;i++){
    sensors[tail]=sensors[i];
    tail++;
  }
  head--;
}

int SensorManager::getNumber(){
  return head;
}

bool SensorManager::checkSensors(){
  for (i=0;i<head;i++){
    if (sensors[i].readFromSensor()){
      return 1;
    }
  }
  return 0;
}



