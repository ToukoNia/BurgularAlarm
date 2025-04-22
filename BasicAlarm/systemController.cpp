#include "systemController.h"
void SystemController::setup(){
  Sensors.addSensor(2,1,3,"Window");  //window
  Sensors.addSensor(4,0,2,"PIR Motion Detector");  //PIR  
  Sensors.changeFobSensor(4,0,1,"Fob Button");  //button
  Locks.addLock(3,"Solenoid");  //solenoid
  Alarm.setupAlarm();
  Users.Setup("Cheese",3,"Nia");
}

void SystemController::armSystem(){
  Locks.lockAll();
  flag=0;
  timeStamp=millis();
  while (timeStamp+ALARM_DELAY*1000>millis()){  //wait after system is armed until user could have logged in
    if (Login()==1){
      Locks.unlockAll();
      return;
    } else if (Login()==-1){
      flag=1;
    }
  } 
  while (!Sensors.checkSensors()&&!flag){ //loops until sensor checked, or login detected, or over login limit
    if (Login()==1){
        Locks.unlockAll();
        return;
    }
    else if (Login()==-1){
      flag=1;
    }
  }
  timeStamp=millis();
  while (timeStamp+ALARM_DELAY*1000>millis()&&!flag){ //delay to allow the user to get there in time
    if (Login()==1){
      Locks.unlockAll();
      return;
    } else if (Login()==-1){
    flag=1;
    }   
  }
  raiseAlarm();
}


bool SystemController::raiseAlarm(){  //simple code that manages maintaining and creating the alarm (note, might be a better idea to destroy the alarm manager and to fit it here)
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

void SystemController::testSystem(){  //test system code will go here
  Locks.lockAll();
  timeStamp=millis();
  raiseAlarm();
}

int SystemController::manageUsers(int type, String userID){ //0 denotes remove user, 1/2 refers to add user of admin level 0/1
  if (type){
    return Users.addUser(userID,type-1);
  } else{
    return Users.removeUser(userID);
  }
}

int SystemController::Login(){  //Login code to facilitate loggin in
  if(communicator.checkSerial("Check Login")){
    username=communicator.getSerial("Check Username");
    password=communicator.getSerial("Check Password");
    return Users.authenticate(username, password);
  }
  return 0;
}

bool SerialController::checkSerial(String prompt){  //checks the serial bus to see if there is a message and if that message equals 1
  Serial.println(prompt);
  if(Serial.available() > 0){
    message = Serial.readStringUntil('\n');
    message.trim();
    return (message=="1");
  }
  return 0;
}

String SerialController::getSerial(String prompt){  //gets the serial by prompting with the above prompt and then manages it
  Serial.println(prompt);
  while (!(Serial.available()>0));  //waits for a response (NOTE: this is only called when the system knows for sure its getting a response)
  message=Serial.readStringUntil("\n");
  message.trim();
  return message;
}
