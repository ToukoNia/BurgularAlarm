#include "deviceManager.h"

bool SensorManager::addSensor(int pinNo, bool logic, int type, String name) {  //adds sensors
  if (head<MAX_IO){ //ensures no memory problem
    sensors[head] = Sensor(); //uses a head pointer to maintain spot in array, tracking the posistion of the next item to be added (as arduino starts at 0)
    sensors[head].createDevice(pinNo, logic, type, name); 
    head++;
    return 1; //return 1 if successful
  } 
  return 0;
}
void SensorManager::changeFobSensor(int pinNo, bool logic, int type, String name) {  //adds fobsensors, seperate as works for unlocking the door
    fobSensor = Sensor();
    fobSensor.createDevice(pinNo, logic, type, name); 
}

bool SensorManager::checkFobSensor(){   //checks the fob sensor, seperate to allow for unlocking a door
  return fobSensor.readFromSensor();
}

void SensorManager::removeSensor(int number) {  //removes a sensor
  tail = number;
  for (i = number + 1; i < head; i++) { //loops through the array from the tail pointer to the current head, switching the posistion of the nth and n+1th term to move the array back by one whilst preserving order
    sensors[tail] = sensors[i];
    tail++; 
  }
  head--; //head is deincremented to reset last position so the system operates like normal
}

int SensorManager::getNumber() {
  return head;
}

bool SensorManager::checkSensors() {
  for (i = 0; i < head; i++) {  //Loops through and returns 1 if a sensor detects something
    if (sensors[i].readFromSensor()) {
      return 1;
    }
  }
  return 0;
}

void SensorManager::displaySensorList(bool infoLevel){  //these two output the list of names of the sensors and locks, with a start message and an end message
  Serial.println("Sensor List Start");
  while (!(Serial.available()>0));  //waits until recieving a response from MATLAB
  for (i = 0; i < head; i++) {
      Serial.print(sensors[i].getName()); //prints the name for every sensor in the array
      if (infoLevel){ 
        Serial.print(","); sensors[i].printOut();   //if needed, also prints out the extra info
      } 
      Serial.println(); //prints a newline to allow for MATLAB to read both outputs uniformly
  }
  Serial.println("Sensor List End");  //tells MATLAB to stop looking
}

void LockManager::displayLockList(bool infoLevel){    //displays the lock list as per above
  Serial.println("Lock List Start");
  while (!(Serial.available()>0));
  for (i = 0; i < head; i++) {
      Serial.print(locks[i].getName());
      if (infoLevel){ 
        Serial.print(","); locks[i].printOut();
      }
      Serial.println();
  }
  Serial.println("Lock List End");
}

void SensorManager::testSensors() { //tests the sensors for use in TestSystem();
  for (i = 0; i < head; i++) {
    if (sensors[i].readFromSensor()) {
      Serial.print("Sensor "); Serial.print(sensors[i].getName());  Serial.println(" Triggered");
    } else{
      Serial.println("No sensor has been triggered");
    }
  }
}

bool LockManager::addLock(int pinNo, String name) { //adds a lock to the lock manager, derivative of addSensor
  if (head<MAX_IO){
    locks[head] = Lock();
    locks[head].createDevice(pinNo, 0, 0,name);  
    head++;
    return 1;
  }
  return 0;
}

void LockManager::removeLock(int number) {  //removes a lock by a variation on the sensor logic
  tail = number;
  for (i = number + 1; i < head; i++) { 
    locks[tail] = locks[i];
    tail++;
  }
  head--;
}

int LockManager::getNumber() {  //gets the number of locks
  return head;
}

void LockManager::lockAll() { //locks all the added locks
  for (i = 0; i < head; i++) {
    locks[i].lock();
  }
}

void LockManager::unlockAll() { //unlocks all the locks
  for (i = 0; i < head; i++) {
    locks[i].unlock();
  }
}

void AlarmManager::setupAlarm() { //sets up the alarm
  speaker.createDevice(BUZZER_PIN, 0, 0,"Buzzer");
  Beacon.createDevice(LED_PIN, 0, 0,"Beacon");
}

void AlarmManager::maintainAlarm() {  //manages blinking the LEDs at a fixed frequency (2Hz at the moment)
  if (timeStamp + 1000 / LEDFrequency <= millis()) {
    Beacon.changeState();
    timeStamp = millis();
  }
}

void AlarmManager::triggerAlarm() { //sounds the alarm
  speaker.sound();
}

void AlarmManager::stopAlarm() {  //stops the alarm
  speaker.stop();
  Beacon.turnOff();
}
