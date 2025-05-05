//SystemController.h
#ifndef SystemController_h
#define SystemController_h
#include <Arduino.h>
#include "users.h"
#include "deviceManager.h"
#define ALARM_LENGTH 10
#define TEST_LENGTH 3
#define LOCKOUT_TIME 30
class SerialController {
  private:
    String message;
    unsigned long timeStamp;
    const unsigned long SERIAL_TIMEOUT = 50; 
  public:
    bool checkSerial(String prompt);
    String getSerial(String prompt);
};

class SystemController { 
  private:
    SensorManager Sensors;
    LockManager Locks;
    AlarmManager Alarm;
    SerialController communicator;
    AuthenticationManager Users;
    String name; String password; String message; String temp; String temp1; String temp2;  //needed for all the serial readIns

    long timeStamp=0; int flag; int value; int login; bool skipLogin=1; bool logic; int i1; int i2; int i3; int alarmDelay=20;
    int armSystem();  //Logics arming the system
    int raiseAlarm();
    void testSystem();
    int Login();
    bool updateDevices(); //all subsequent private functions were functionalised to make code more readible, but are not used as functions. Manage getting data from SerialController and combining with other classes to aid full system
    void updateUsers();
    void updateCredentials();
    void createSystem();
    void loadSystem();
  public:
    void setup(); //creates the system
    void fullSystem();  //runs the full system.
   
};


#endif