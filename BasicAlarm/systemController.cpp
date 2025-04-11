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
  while (timeStamp+ALARM_DELAY*1000>millis()){
    if (Login()){
      Locks.unlockAll();
      return;
    }
  } 
  while (!Sensors.checkSensors());  //&& !checkLogin
  if (Sensors.checkSensors()){
    timeStamp=millis();
    while (timeStamp+ALARM_DELAY*1000>millis()){
    if (Login()){
      Locks.unlockAll();
      return;
    }
    }
    raiseAlarm();
  }
}

bool SystemController::raiseAlarm(){
    Alarm.triggerAlarm();
    timeStamp=millis();
    while(millis()<timeStamp+ALARM_LENGTH*1000){
      Alarm.maintainAlarm();
      if (Login()){
        Locks.unlockAll();
        Alarm.stopAlarm();
        return 1;
      }
    }
    Alarm.stopAlarm(); 
    return 0;
}

void SystemController::testSystem(){
  Locks.lockAll();
  timeStamp=millis();
  raiseAlarm();
}

bool SystemController::Login(){
  if(communicator.checkLogin()){
    username=communicator.checkUsername();
    password=communicator.checkPassword();
  } if (password=="Cheese"){
    return 1;
  } else{
    return 0;
  }
}

String SerialController::checkState(){
  Serial.println("Check State");
  while (!(Serial.available()>0));
  message=Serial.readStringUntil("\n");
  message.trim();
  return message;
}

bool SerialController::checkLogin(){
  Serial.println("Check Login");
  if(Serial.available() > 0){
    message = Serial.readStringUntil('\n');
    message.trim();
    return (message=="1");
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
