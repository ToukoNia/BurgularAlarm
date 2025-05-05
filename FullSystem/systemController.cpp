#include "systemController.h"
void SystemController::setup(){
  message=communicator.getSerial("Setup Option");
  if (message=="LOAD"){
    loadSystem();
  } else {
    createSystem();
  }
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
          Serial.println("Problem Occurred");
        }
      } else if (message=="C"){
        updateCredentials();
      }
      else if (message=="L"){
        flag=0;
      }
      else if (message=="SE"){
        //SEND SENSOR DATA
        Sensors.displaySensorList(0);
      }
      else if (message=="LO"){
        //SEND LOCK DATA
        Locks.displayLockList(0);
      } else if (message=="S"){
        Users.printOut();
        Sensors.displaySensorList(1);
        Locks.displayLockList(1);
      }else if (message=="UT"){
        message=communicator.getSerial("Get Delay");
        alarmDelay = message.toInt();
      }
    }
  } else if (login==-1){  //
    timeStamp=millis();
    while(millis()<timeStamp+LOCKOUT_TIME*1000){
      Serial.println("System Locked");
    }
    Users.resetAttempts();
  }
}


int SystemController::armSystem(){
  Locks.lockAll();
  flag=0;
  timeStamp=millis();
  while (!Sensors.checkSensors()&&!flag){ //loops until sensor checked, or login detected, or over login limit
    value=Login();
    if (value>0){
      Locks.unlockAll();
      return value;
    } else if (value==-1){
      flag=1; //basically if too many logins set off alarm
    } if (Sensors.checkFobSensor()){
      Locks.unlockAll();
    }
  }
  timeStamp=millis();
  while (timeStamp+alarmDelay*1000>millis()&&!flag){ //delay to allow the user to get there in time, except if the flag is set (too many logins)
    value=Login();
    if (value>0){
      Locks.unlockAll();
      return value;
    } else if (value==-1){
      flag=1;
    }
    if (Sensors.checkFobSensor()){
      Locks.unlockAll();
    }   
  }
  value=raiseAlarm();
  if (value>0){
    Locks.unlockAll();
    return value;
  } else if (value==-1){
    timeStamp=millis();
    while(millis()<timeStamp+LOCKOUT_TIME*1000){
      Serial.println("System Locked");
    }
    Users.resetAttempts();
  }
}

int SystemController::raiseAlarm(){  //simple code that manages maintaining and creating the alarm (note, might be a better idea to destroy the alarm manager and to fit it here)
    Alarm.triggerAlarm();
    timeStamp=millis();
    while(millis()<timeStamp+ALARM_LENGTH*1000){
      Alarm.maintainAlarm();
      value=Login();
      if (value==1){
        Locks.unlockAll();
        Alarm.stopAlarm();
        return value;
      }
    }
    Alarm.stopAlarm(); 
    return 0;
}

void SystemController::loadSystem(){
  message=communicator.getSerial("Password");
  value=communicator.getSerial("Attempts").toInt();
  Users.updateMaxAttempts(value);
  Users.updatePin(message);

  //Loads the users
  message="";
  while(message!="Users End"){
    message=communicator.getSerial("NEXT");
    i1 = message.indexOf(',');
    if (i1 != -1) {
      name = message.substring(0, i1);
      logic = bool(message.substring(i1 + 1).toInt());
      Users.addUser(name,logic);
    }
  }

  //Loads the sensors
  while(message!="Sensors End"){
  message=communicator.getSerial("NEXT");
  i1 = message.indexOf(',');
    if (i1 != -1) {
      i2 = message.indexOf(',',i1+1);
      i3 = message.indexOf(',',i2+1);
      name = message.substring(0, i1);
      value = message.substring(i1+1,i2).toInt();
      logic = message.substring(i2+1,i3).toInt();
      flag = message.substring(i3+1).toInt();
      Sensors.addSensor(value, logic, flag,name);
    }
  }

  //Loads the locks
  while(message!="Locks End"){
    message=communicator.getSerial("NEXT");
    Serial.println(message);
    i1 = message.indexOf(',');
    if (i1 != -1) {
      name = message.substring(0, i1);
      value = message.substring(i1 + 1).toInt();
      Locks.addLock(value,name);
    }
  }
  
  Alarm.setupAlarm();
  Sensors.changeFobSensor(5,1,1,"Fob Button");  //button
}

void SystemController::createSystem(){  //creates a basic system in the event of no system being loaded
  updateCredentials();
  updateUsers();  //need to also send add command
  updateDevices();  //need to also send add command, needs to send over specified sensor/lock etc
  updateDevices();  //need to also send add command, specify sensor/lock
  Alarm.setupAlarm();
  Sensors.changeFobSensor(5,1,1,"Fob Button");  //button
}

void SystemController::testSystem(){  //tests the system in line with the system requirements
  Serial.println("Testing the locks");
  Locks.lockAll();
  delay(TEST_LENGTH*1000);
  Locks.unlockAll();
  timeStamp=millis();
  Serial.println("Testing the Alarm");
  Alarm.triggerAlarm();
  timeStamp=millis();
  while(millis()<timeStamp+TEST_LENGTH*1000){
    Alarm.maintainAlarm();
  }
  Alarm.stopAlarm(); 
  timeStamp=millis();
    Serial.println("Testing the Sensors, trigger them to see on screen");
  delay(500);
  while(millis()<timeStamp+2*TEST_LENGTH*1000){
    Sensors.testSensors();
  }
  Serial.println("Test Complete");
}

void SystemController::updateCredentials(){
  password=communicator.getSerial("New Password");
  message=communicator.getSerial("Attempt Change");
  Users.updateMaxAttempts(message.toInt());
  Users.updatePin(password);
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
    value = Users.addUser(name,(message=="1"));  //add a new user w the permissions of the above message
    if (value==1){  //error/success messages
      Serial.print(name); Serial.println(" has been successfully added.");
    } else {
      Serial.println("You are at the maximum number of users. Please delete a user before adding a new one.");
    }
  } else{//put something to print out all users
    value=Users.removeUser(name);  //delete the user
    if (value==1){  //error messages
      Serial.print(name); Serial.println(" has been successfully deleted.");
    } else if (value==-1){
      Serial.print(name); Serial.println(" is the current user, and so cannot be deleted.");
    } else{
      Serial.print(name); Serial.println(" is not a valid user.");
    }
  }
}

bool SystemController::updateDevices(){ //might lowkey wanna switch this to be without so many temps lmao
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
      temp=communicator.getSerial("Sensor Number"); //gets the sensor number in teh array and removes it
      Sensors.removeSensor(temp.toInt());
      return 1;
    }
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
  message=Serial.readStringUntil('\n');
  message.trim();
  return message;
}

