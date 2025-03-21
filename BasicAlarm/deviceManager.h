//deviceManager.h
#ifndef deviceManager_h
#define deviceManager_h
#include <Arduino.h>
#include "device.h"

class SensorManager { 
  private:
    int head=0; int tail=-1; int i;
    Sensor* sensors;
  public:
    void addSensor(int pinNo);
    void removeSensor(int number);
    bool checkSensors();
    int getNumber();
};

class LockManager{
  private:
    int head=0; int tail=-1;
    Lock* locks;
  public:
    void addLock(int pinNo);
    void removeLock(int pinNo);
    void unlockAll();
    void lockAll();
};

class AlarmManager{
  private:
    bool alarmSounding;
    Buzzer speaker;
    LED Beacon;
    int LEDFrequency;
  public:
    void triggerAlarm();
    void stopAlarm();
    bool getStatus();
    void testAlarm();
    void setupAlarm();
};

#endif