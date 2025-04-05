#include "systemController.h"
#include "users.h"
#define ALARM_DELAY 10
SystemController sys;
SerialController communicator;
String username; String password;
int i;
void setup() {
  sys.setup();
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(communicator.checkLogin()){
    username=communicator.checkUsername();
    password=communicator.checkPassword();
  };
  if (password=="Cheese"){
    digitalWrite(3,1);
  }
}



