//SystemController.h
#ifndef SystemController_h
#define SystemController_h
#include <Arduino.h>
#include "deviceManager.h"
#define ALARM_DELAY 10
#define ALARM_LENGTH 1

class SystemController { 
  private:
    SensorManager Sensors;
    LockManager Locks;
    AlarmManager Alarm;
    long timeStamp=0; 
  public:
    void setup();
    void armSystem();
    void raiseAlarm();
    void testSystem();
};

class SerialController {
  private:
    String message;
  public:
    bool checkLogin();
    String checkState();
    void changeDisplay();
    String checkUsername();
    String checkPassword();
};
#endif