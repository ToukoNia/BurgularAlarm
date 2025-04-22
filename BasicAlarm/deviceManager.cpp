#include "deviceManager.h"

bool SensorManager::addSensor(int pinNo, bool logic, int type, String name) {  //adds sensors
  if (head<MAX_IO){
    sensors[head] = Sensor();
    sensors[head].createDevice(pinNo, logic, type, name);  //change to constructors at somepoint
    head++;
    return 1;
  }
  return 0;
}

void SensorManager::changeFobSensor(int pinNo, bool logic, int type, String name) {  //adds fobsensors
    fobSensor = Sensor();
    sensors[head].createDevice(pinNo, logic, type, name);  //change to constructors at somepoint
}

void SensorManager::removeSensor(int number) {
  tail = number;
  for (i = number + 1; i < head; i++) {
    sensors[tail] = sensors[i];
    tail++;
  }
  head--;
}

int SensorManager::getNumber() {
  return head;
}

bool SensorManager::checkSensors() {
  for (i = 0; i < head; i++) {
    if (sensors[i].readFromSensor()) {
      return 1;
    }
  }
  return 0;
}

bool LockManager::addLock(int pinNo, String name) {
  if (head<MAX_IO){
    locks[head] = Lock();
    locks[head].createDevice(pinNo, 0, 0,name);  //change to constructors at somepoint
    head++;
    return 1;
  }
  return 0;
}

void LockManager::removeLock(int number) {
  tail = number;
  for (i = number + 1; i < head; i++) {
    locks[tail] = locks[i];
    tail++;
  }
  head--;
}

int LockManager::getNumber() {
  return head;
}

void LockManager::lockAll() {
  for (i = 0; i < head; i++) {
    locks[i].lock();
  }
}

void LockManager::unlockAll() {
  for (i = 0; i < head; i++) {
    locks[i].unlock();
  }
}

void AlarmManager::setupAlarm() {
  speaker.createDevice(BUZZER_PIN, 0, 0,"Buzzer");
  Beacon.createDevice(LED_PIN, 0, 0,"Beacon");
}

void AlarmManager::maintainAlarm() {
  if (timeStamp + 1000 / LEDFrequency <= millis()) {
    Beacon.changeState();
    timeStamp = millis();
  }
}

void AlarmManager::triggerAlarm() {
  speaker.sound();
}

void AlarmManager::stopAlarm() {
  speaker.stop();
  Beacon.turnOff();
}
