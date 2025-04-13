#include "systemController.h"
#include "users.h"
#define ALARM_DELAY 10
SystemController sys;
SerialController communicator;
String message; String username;
bool flag;
void setup() {
  sys.setup();
  sys.manageUsers(1,"Mumin");
  Serial.begin(9600);
}

void loop() {
  
  int login=sys.Login();
  if (login==1){
    flag=1;
    while (flag){
      message=communicator.getSerial("User Logged in");
      if (message=="A"){
        digitalWrite(3,1);
      } else if (message=="L"){
        flag=0;
      }
    }
    
  } else if (login==2){
    flag=1;
    while(flag){
      message=communicator.getSerial("Admin Logged in");
      if (message=="A"){
        digitalWrite(3,1);
      } else if (message=="T"){
        digitalWrite(4,1);
      } else if (message=="U"){
        message=communicator.getSerial("Add or Remove");
        username=communicator.getSerial("Get Username");
        if (message=="A"){
          message=communicator.getSerial("Get Admin");
          sys.manageUsers((1+(message=="1")), username);
        } else{//put something to print out all users
            if (sys.manageUsers(0,username)){
              Serial.print(username); Serial.println(" has been successfully deleted.");
            } else{
              Serial.print(username); Serial.println(" is not a valid user.");
            }
          }
      }
      else if (message=="L"){
        flag=0;
      }
    }
  } 
}



