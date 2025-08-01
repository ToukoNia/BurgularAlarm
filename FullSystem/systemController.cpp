#include "systemController.h"
void SystemController::setup(){ //sets up the system
  message=communicator.getSerial("Setup Option");
  if (message=="LOAD"){
    loadSystem();
  } else {
    createSystem();
  }
}

void SystemController::fullSystem(){
  if(!skipLogin){ //allows for login to be skipped (used when logging in from disarming the system to prevent needing to sign in twice)
    login=Login();  //logs in
  } else{
    skipLogin=0;
  }
  
  if (login==1){  //if user logged in
    flag=1;
    while (flag){
      message=communicator.getSerial("User Logged in");
      if (message=="A"){  
        flag=0; //log the user out
        login=armSystem();  //arm the system
        skipLogin=1;  //skip nxt login as mentioned above
        
      } else if (message=="L"){
        flag=0; //logout
      }
    }
    
  } else if (login==2){ //if an admin logged in
    flag=1;
    while(flag){
      message=communicator.getSerial("Admin Logged in");
      if (message=="A"){  //arm the system, same as user
        login=armSystem();
        skipLogin=1;
        flag=0;
      } else if (message=="T"){ //test the system
        testSystem();
      } else if (message=="U"){ //update credentials
        updateUsers();
      } else if (message=="D"){ //updates the devices, and then outputs to the MATLAB side
        if (updateDevices()){
          Serial.println("Operation Successful");
        } else {
          Serial.println("Problem Occurred");
        }
      } else if (message=="C"){
        updateCredentials();
      }
      else if (message=="L"){//logout
        flag=0;
      }
      else if (message=="SE"){ //send sensor data in a limited format for input to a table
        Sensors.displaySensorList(0);
      }
      else if (message=="LO"){ //send lock data in a limited format for input to a table
        Locks.displayLockList(0);
      } else if (message=="S"){ 
        Users.printOut(); //prints the credentials, then the users, then the sensors, then the locks in the full format. This is then saved by the MATLAB side into corresponding text files
        Sensors.displaySensorList(1);
        Locks.displayLockList(1);
      }else if (message=="UT"){ //updates the delay
        message=communicator.getSerial("Get Delay");
        alarmDelay = message.toInt();
      }
    }
  } else if (login==-1){  //if system locked out, wait for the full duration before allowing subsequent login attempts
    timeStamp=millis();
    while(millis()<timeStamp+LOCKOUT_TIME*1000){
      Serial.println("System Locked");
    }
    Users.resetAttempts();  //reset attempts
  }
}

int SystemController::armSystem(){  //arms the system in accordance to the system requirements
  Locks.lockAll();
  flag=0;
  timeStamp=millis();
  while (!Sensors.checkSensors()&&!flag){ //loops until sensor checked, or login detected, or over login limit
    value=Login();
    if (value>0){
      Locks.unlockAll();  //successful logins turn off the alarm and logs out
      return value;
    } else if (value==-1){
      flag=1; //basically if too many logins set off alarm
    }    
    //door and fob sensor 
    if (Sensors.checkFobSensor()){  //if button pushed unlock door
      Locks.unlockAll();
      lockTimeStamp=millis();
      logic=1;
    }
    if(logic){
      if (millis()>lockTimeStamp+5000){ //5 seconds later, relock the door
        Locks.lockAll();
        logic=0;
      }
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
    if (Sensors.checkFobSensor()){  //if button pushed unlock door
      Locks.unlockAll();
      lockTimeStamp=millis();
      logic=1;
    }
    if(logic){
      if (millis()>lockTimeStamp+5000){ //5 seconds later
        Locks.lockAll();
        logic=0;
      }
    }
  }
  value=raiseAlarm();
  while(!(value>0)){  //in the event of still no login, stay till successful login.
    if (value>0){ //keep doors locked until login successful
      Locks.unlockAll();
      return value;
    } else if (value==-1){  //reset attempts
      timeStamp=millis();
      while(millis()<timeStamp+LOCKOUT_TIME*1000){
        Serial.println("System Locked");
      }
      Users.resetAttempts();
    }
    if (Sensors.checkFobSensor()){  //if button pushed unlock door
      Locks.unlockAll();
      lockTimeStamp=millis();
      logic=1;
    }
    if(logic){
      if (millis()>lockTimeStamp+5000){ //5 seconds later
        Locks.lockAll();
        logic=0;
      }
    }
    value=Login();
  }
  
}

int SystemController::raiseAlarm(){  //simple code that manages maintaining and creating the alarm
    Alarm.triggerAlarm();
    timeStamp=millis();
    while(millis()<timeStamp+ALARM_LENGTH*1000){  
      Alarm.maintainAlarm();  //flashes LED
      value=Login();
      if (value==1){  //if logged in, unlockAll and stop alarm
        Locks.unlockAll();
        Alarm.stopAlarm();
        return value;
      }
      if (Sensors.checkFobSensor()){  //if button pushed unlock door, same as in arm system
        Locks.unlockAll();
        lockTimeStamp=millis();
        logic=1;
      }
      if(logic){
        if (millis()>lockTimeStamp+5000){ //5 seconds later
          Locks.lockAll();
          logic=0;
        }
      }  
    }
    Alarm.stopAlarm(); 
    return 0; //if no login, return 0
}

void SystemController::loadSystem(){  //loads the system from files stored on the MATLAB side
  //loads the credentials
  updateCredentials();

  //Loads the users in the format "NAME,ADMIN_LEVEL"
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

  //Loads the sensors in the format "NAME, PIN NUMBER, LOGIC, TYPE". Variable names are as follows purely as they are temp variables that were previously used to save on memory
  while(message!="Sensors End"){
  message=communicator.getSerial("NEXT");
  i1 = message.indexOf(',');
    if (i1 != -1) {
      i2 = message.indexOf(',',i1+1);
      i3 = message.indexOf(',',i2+1);
      name = message.substring(0, i1);  
      value = message.substring(i1+1,i2).toInt(); //pin number
      logic = message.substring(i2+1,i3).toInt();
      flag = message.substring(i3+1).toInt(); //type of device (ie input_pullup or input)
      Sensors.addSensor(value, logic, flag,name);
    }
  }

  //Loads the locks in the format (Name,Pin number). Value was reused for pin number as it is a temporary variable
  while(message!="Locks End"){
    message=communicator.getSerial("NEXT");
    i1 = message.indexOf(',');
    if (i1 != -1) {
      name = message.substring(0, i1);
      value = message.substring(i1 + 1).toInt();  //pin number
      Locks.addLock(value,name);
    }
  }
  
  Alarm.setupAlarm(); //sets up the alarm
  Sensors.changeFobSensor(5,1,1,"Fob Button");  //button
}

void SystemController::createSystem(){  //creates a basic system in the event of no system being loaded
  updateCredentials();  //creates credentials
  updateUsers();   //gets a singular user
  updateDevices();   //gets a singular sensor (option selected MATLAB side)
  updateDevices();  //gets a singular lock (option selected MATLAB side)
  Alarm.setupAlarm(); 
  Sensors.changeFobSensor(5,1,1,"Fob Button");  //button
}

void SystemController::testSystem(){  //tests the system in line with the system requirements
  Serial.println("Testing the locks");
  Locks.lockAll();
  delay(TEST_LENGTH*1000);
  Locks.unlockAll();
  timeStamp=millis();
  Serial.println("Testing the Alarm");  //outputs for MATLAB
  Alarm.triggerAlarm();
  timeStamp=millis();
  while(millis()<timeStamp+TEST_LENGTH*1000){ //flashes LED
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

void SystemController::updateCredentials(){ //updates the password based on stuff from the GUI
  password=communicator.getSerial("New Password");
  message=communicator.getSerial("Attempt Change");
  if (message!="0"){  //flags for an unchanged password from the gui
    Users.updateMaxAttempts(message.toInt());
  }
  if (password!="0"){
    Users.updatePin(password);
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

bool SystemController::updateDevices(){ //takes in the inputs from MATLAB to handle updating a device through the device managers. Temp variables are used throughout with comments for readibility.
  message=communicator.getSerial("Add or remove");
  if (message=="A"){
    message=communicator.getSerial("Sensor or Lock");  //gets the selected options from matlab
    temp=communicator.getSerial("Pin Number");  //holds the pin number
    name=communicator.getSerial("Name");
    if (message=="Sensor"){
      temp1=communicator.getSerial("Logic");  //1 for logic of the sensor
      temp2=communicator.getSerial("Standard or Pullup"); //0 for if the sensor is input pullup or 1 for pullup 
      return Sensors.addSensor(temp.toInt(), (temp1=="1"), (temp2=="1"+1), name);  
    } else {
      return Locks.addLock(temp.toInt(),name);  //assigns the values
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

bool SerialController::checkSerial(String prompt){  //checks the serial bus to see if there is a message and if that message equals 1, used for logging in
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

