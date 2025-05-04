//SystemController.h
#ifndef SystemController_h
#define SystemController_h
#include <Arduino.h>
#include "users.h"
#include "deviceManager.h"
#define ALARM_DELAY 20
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
    String getArrayValues();
};

class SystemController { 
  private:
    SensorManager Sensors;
    LockManager Locks;
    AlarmManager Alarm;
    SerialController communicator;
    AuthenticationManager Users;
    String name; String password; String message; String temp; String temp1; String temp2;  //needed for all the serial readIns
    long timeStamp=0; int flag; int value; int login; bool skipLogin=1; bool logic;
    int armSystem();
    int raiseAlarm();
    void testSystem();
    int Login();
    int manageUsers(int type, String userID);
    bool updateDevices();
    void updateUsers();
    void updateCredentials();
   
  public:
    void setup();
    void fullSystem();
    void loadSystem();
   
};


#endif