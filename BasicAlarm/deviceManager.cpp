#include "deviceManager.h"

void SensorManager::addSensor(int pinNo,int type){
  sensors[head]=Sensor();
  sensors[head].createDevice(pinNo,0,type); //change to constructors at somepoint
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

void LockManager::addLock(int pinNo){
  locks[head]=Lock();
  locks[head].createDevice(pinNo,0,1); //change to constructors at somepoint
  head++;
}

void LockManager::removeLock(int number){
  tail=number;
  for (i=number+1;i<head;i++){
    locks[tail]=locks[i];
    tail++;
  }
  head--;
}

int LockManager::getNumber(){
  return head;
}

void LockManager::lockAll(){
  for (i=0;i<head;i++){
    locks[i].lock();
  }
}

void LockManager::unlockAll(){
  for (i=0;i<head;i++){
    locks[i].unlock();
  }
}

void AlarmManager::setupAlarm(){
  speaker.createDevice(BUZZER_PIN,0,1);
  Beacon.createDevice(LED_PIN,0,1);
}

void AlarmManager::maintainAlarm(){
  if (timeStamp+1000/LEDFrequency<=millis()){
    Beacon.changeState();
      timeStamp=millis();

  }
}

void AlarmManager::triggerAlarm(){
  speaker.sound();
}

void AlarmManager::stopAlarm(){
  speaker.stop();
}

