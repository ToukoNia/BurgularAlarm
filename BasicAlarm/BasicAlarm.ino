#include "systemController.h"
#include "users.h"
#define ALARM_DELAY 10
SystemController sys;
SerialController communicator;
String message; String username; String temp; String temp1; String temp2;
bool flag; int value;
void setup() {
  sys.setup();
  sys.manageUsers(1,"Mumin");
  Serial.begin(9600);
}

void loop() {
  //basic TUI, needs to have the facial recognition implemented and add the ability to change the number of attempts, pin, sensors, locks etc
  int login=sys.Login();  //logs in
  if (login==1){  //if user logged in
    flag=1;
    while (flag){
      message=communicator.getSerial("User Logged in");
      if (message=="A"){
        digitalWrite(3,1);
      } else if (message=="L"){
        flag=0;
      }
    }
    
  } else if (login==2){ //if an admin logged in
    flag=1;
    while(flag){
      message=communicator.getSerial("Admin Logged in");
      if (message=="A"){  //arm the system
        digitalWrite(3,1);
      } else if (message=="T"){ //test the system
        digitalWrite(4,1);
      } else if (message=="U"){ //update credentials
        sys.updateUsers();
      } else if (message=="D"){
        sys.updateDevices();
      }
      else if (message=="L"){
        flag=0;
      }
    }
  } 
}





