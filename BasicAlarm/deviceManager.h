//deviceManager.h
#ifndef deviceManager_h
#define deviceManager_h
#include <Arduino.h>
#include "device.h"
#define MAX_IO 12

#define BUZZER_PIN 7
#define LED_PIN 6

class SensorManager { 
  private:
    int head=0; int tail=0; int i;
    Sensor sensors[MAX_IO];
    Sensor fobSensor;
  public:
    bool addSensor(int pinNo, bool logic, int type, String name);
    void removeSensor(int number);
    void changeFobSensor(int pinNo, bool logic, int type, String name);
    bool checkSensors();
    int getNumber();
    bool checkFobSensor();
};

class LockManager{
  private:
    int head=0; int tail=0; int i;
    Lock locks[MAX_IO];
  public:
    bool addLock(int pinNo, String name);
    void removeLock(int pinNo);
    int getNumber();
    void unlockAll();
    void lockAll();
};

class AlarmManager{
  private:
    bool alarmSounding;
    Buzzer speaker;
    LED Beacon;
    int LEDFrequency=2;
    long timeStamp;
  public:
    void triggerAlarm();
    void maintainAlarm();
    void stopAlarm();
    bool getStatus();
    void testAlarm();
    void setupAlarm();
};

#endif