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
    name=communicator.getSerial("Check Username");
    password=communicator.getSerial("Check Password");
    return Users.authenticate(name, password);
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
  message=Serial.readStringUntil("\n");
  message.trim();
  return message;
}
