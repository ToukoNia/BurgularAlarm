#include "systemController.h"
void SystemController::setup(){
  Sensors.addSensor(2,1,3);  //window
  Sensors.addSensor(4,0,2);  //PIR  
  //Sensors.addSensor(4,0,1);  //button
  Locks.addLock(3);  //solenoid
  Alarm.setupAlarm();
}

void SystemController::armSystem(){
  Locks.lockAll();
  timeStamp=millis();
  while (timeStamp+ALARM_DELAY*1000>millis()); //checkLogin
  while (!Sensors.checkSensors());  //&& !checkLogin
  if (Sensors.checkSensors()){
    timeStamp=millis();
    while (timeStamp+ALARM_DELAY*1000>millis()){
      //check login code
    }
    raiseAlarm();
  }
}

void SystemController::raiseAlarm(){
    Alarm.triggerAlarm();
    timeStamp=millis();
    while(millis()<timeStamp+ALARM_LENGTH*1000){
      Alarm.maintainAlarm();
      //check login Code
    }
    Alarm.stopAlarm(); 
}

void SystemController::testSystem(){
  Locks.lockAll();
  timeStamp=millis();
  raiseAlarm();
}


String SerialController::checkState(){
  Serial.println("Check Login");
  while (!(Serial.available()>0));
  message=Serial.readStringUntil("\n");
  message.trim();
  return message;
}

bool SerialController::checkLogin(){
  Serial.println("Check Login");
  while (!(Serial.available()>0));
  message=Serial.readStringUntil("\n");
  message.trim();
  if (message=="1"){
    return 1;
  } 
  return 0;
}

String SerialController::checkUsername(){
  Serial.println("Check Username");
  while (!(Serial.available()>0));
  message=Serial.readStringUntil("\n");
  message.trim();
  return message;
}

String SerialController::checkPassword(){
  Serial.println("Check Password");
  while (!(Serial.available()>0));  //timeout to be added
  message=Serial.readStringUntil("\n");
  message.trim();
  return message;
}