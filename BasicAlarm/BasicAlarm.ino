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
  if (sys.Login()){
    digitalWrite(3,1);
  }
}



