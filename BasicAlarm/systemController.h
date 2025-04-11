//SystemController.h
#ifndef SystemController_h
#define SystemController_h
#include <Arduino.h>
#include "deviceManager.h"
#define ALARM_DELAY 10
#define ALARM_LENGTH 1

class SerialController {
  private:
    String message;
    unsigned long timeStamp;
    const unsigned long SERIAL_TIMEOUT = 50; 
  public:
    bool checkLogin();
    String checkState();
    void changeDisplay();
    String checkUsername();
    String checkPassword();
};

class SystemController { 
  private:
    SensorManager Sensors;
    LockManager Locks;
    AlarmManager Alarm;
    SerialController communicator;
    String username; String password;
    long timeStamp=0; 
  public:
    void setup();
    void armSystem();
    bool raiseAlarm();
    void testSystem();
    bool Login();
};


#endif