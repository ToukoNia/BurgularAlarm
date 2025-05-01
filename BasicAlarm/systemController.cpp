#include "systemController.h"
void SystemController::setup(){
  Sensors.addSensor(2,1,2,"Window");  //window
  Sensors.addSensor(4,0,2,"PIR Motion Detector");  //PIR  
  Sensors.changeFobSensor(5,1,1,"Fob Button");  //button
  Locks.addLock(3,"Solenoid");  //solenoid
  Alarm.setupAlarm();
  Users.Setup("Cheese",3,"Nia");
  manageUsers(1,"Mumin");
  manageUsers(1,"Callum");
}

void SystemController::fullSystem(){
  if(!skipLogin){
    login=Login();  //logs in
  } else{
    skipLogin=0;
  }
  
  if (login==1){  //if user logged in
    flag=1;
    while (flag){
      message=communicator.getSerial("User Logged in");
      if (message=="A"){
        flag=0;
        login=armSystem();
        skipLogin=1;
        
      } else if (message=="L"){
        flag=0;
      }
    }
    
  } else if (login==2){ //if an admin logged in
    flag=1;
    while(flag){
      message=communicator.getSerial("Admin Logged in");
      if (message=="A"){  //arm the system
        login=armSystem();
        skipLogin=1;
        flag=0;
      } else if (message=="T"){ //test the system
        testSystem();
      } else if (message=="U"){ //update credentials
        updateUsers();
      } else if (message=="D"){
        if (updateDevices()){
          Serial.println("Operation Successful");
        } else {
          Serial.println("Problem Occurred")
        }
      } else if (message=="C"){
        updateCredentials();
      }
      else if (message=="L"){
        flag=0;
      }
    }
  } else if (login==-1){  //
    timeStamp=millis();
    while(millis()<timeStamp+LOCKOUT_TIME){
      Serial.println("System Locked");
    }
    Users.resetAttempts();
  }
}


int SystemController::armSystem(){
  Locks.lockAll();
  flag=0;
  timeStamp=millis();/*
  while (timeStamp+ALARM_DELAY*1000>millis()){  //wait after system is armed until user could have logged in, was done on MATLAB side to allow for a smoother countdown and an abort button
    value=Login();
    if (value){
      Locks.unlockAll();
      return value;
    } else if (value==-1){
      flag=1; //might need to be changed lmaoo, but currently will set off the alarm after a bit
    }
  } */
  while (!Sensors.checkSensors()&&!flag){ //loops until sensor checked, or login detected, or over login limit
    value=Login();
    if (value){
      Locks.unlockAll();
      return value;
    } else if (value==-1){
      flag=1; //basically if too many logins set off alarm
    } if (Sensors.checkFobSensor()){
      Locks.unlockAll();
    }
  }
  timeStamp=millis();
  while (timeStamp+ALARM_DELAY*1000>millis()&&!flag){ //delay to allow the user to get there in time, except if the flag is set (too many logins)
    value=Login();
    if (value){
      Locks.unlockAll();
      return value;
    } else if (value==-1){
      flag=1;
    }
    if (Sensors.checkFobSensor()){
      Locks.unlockAll();
    }   
  }
  return raiseAlarm();
}

int SystemController::raiseAlarm(){  //simple code that manages maintaining and creating the alarm (note, might be a better idea to destroy the alarm manager and to fit it here)
    Alarm.triggerAlarm();
    timeStamp=millis();
    while(millis()<timeStamp+ALARM_LENGTH*1000){
      Alarm.maintainAlarm();
      value=Login();
      if (value){
        Locks.unlockAll();
        Alarm.stopAlarm();
        return value;
      }
    }
    Alarm.stopAlarm(); 
    return 0;
}

void SystemController::testSystem(){  //test system code will go here
  Locks.lockAll();
  delay(TEST_LENGTH);
  Locks.unlockAll();
  timeStamp=millis();
  Alarm.triggerAlarm();
  timeStamp=millis();
  while(millis()<timeStamp+TEST_LENGTH*1000){
    Alarm.maintainAlarm();
  }
  Alarm.stopAlarm(); 
  timeStamp=millis();
}

void SystemController::updateCredentials(){
  password=communicator.getSerial("New Password");
  message=communicator.getSerial("Attempt Change");
  Users.updateMaxAttempts(message.toInt());
  Users.updatePin(password);
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
    name=communicator.getSerial("Check Username");
    password=communicator.getSerial("Check Password");
    value=Users.authenticate(name, password);
    if (value==-1){
      Serial.println("Locked");  //flag that system should display as locked on MATLAB side
    } else{
      Serial.println("Not Locked");
    }
    return value;
  }
  return 0;
}

void SystemController::updateUsers(){
  message=communicator.getSerial("Add or Remove");  //gets the selected options from matlab
  name=communicator.getSerial("Get Username");
  if (message=="A"){
    message=communicator.getSerial("Get Admin");  //gets if its an admin signing in
    value = manageUsers((1+(message=="1")), name);  //add a new user w the permissions of the above message
    if (value==1){  //error/success messages
      Serial.print(name); Serial.println(" has been successfully added.");
    } else {
      Serial.println("You are at the maximum number of users. Please delete a user before adding a new one.");
    }
  } else{//put something to print out all users
    value=manageUsers(0,name);  //delete the user
    if (value==1){  //error messages
      Serial.print(name); Serial.println(" has been successfully deleted.");
    } else if (value==-1){
      Serial.print(name); Serial.println(" is the current user, and so cannot be deleted.");
    } else{
      Serial.print(name); Serial.println(" is not a valid user.");
    }
  }
}

void SystemController::updateDevices(){ //might lowkey wanna switch this to be without so many temps lmao
  message=communicator.getSerial("Add or remove");
  if (message=="A"){
    message=communicator.getSerial("Sensor or Lock");  //gets the selected options from matlab
    temp=communicator.getSerial("Pin Number");
    name=communicator.getSerial("Name");
    if (message=="Sensor"){
      temp1=communicator.getSerial("Logic");//1 for logic
      temp2=communicator.getSerial("Standard or Pullup");//1 for pullup
      return Sensors.addSensor(temp.toInt(), (temp1=="1"), (temp2=="1"+1), name);  
    } else {
      return Locks.addLock(temp.toInt(),name);
    }
  }
  else{
    //code to printout all of the desired or smthing (note: has to be in the if section or after it)
    message=communicator.getSerial("Sensor or Lock");  //gets the selected options from matlab
    if (message=="S"){
      Sensors.displaySensorList();
      temp=communicator.getSerial("Sensor Number"); //gets the sensor number in teh array and removes it
      Sensors.removeSensor(temp.toInt());
      return 1;
    }
    Locks.displayLockList();
    temp=communicator.getSerial("Lock Number"); //same with locks
    Locks.removeLock(temp.toInt());
    return 1;
  }
  
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
