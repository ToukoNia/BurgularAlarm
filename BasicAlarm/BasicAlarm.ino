#include "deviceManager.h"
#define ALARM_DELAY 10
SensorManager Jeremy;
LockManager Beremy;

AlarmManager Mistake;
long timeStamp=0;


void setup() {
  // put your setup code here, to run once:
  //solenoid 3
  //pin 5 button
  Serial.begin(9600);
 
  
  Jeremy.addSensor(2,0);  //window
  Jeremy.addSensor(4,0);  //PIR  
//  Jeremy.addSensor(4,2);  //button

  Beremy.addLock(3);  //solenoid

  Mistake.setupAlarm();
}

void loop() {
  // put your main code here, to run repeatedly:
  while (!Jeremy.checkSensors());
  timeStamp=millis();
  while (timeStamp+ALARM_DELAY*1000<millis());
  if (Jeremy.checkSensors()){
    alarm();
  }
  delay(10000);
}

void alarm(){
    Mistake.triggerAlarm();
    timeStamp=millis();
    while(millis()<timeStamp+1000){
      Mistake.maintainAlarm();
    }
    Mistake.stopAlarm(); 
}
