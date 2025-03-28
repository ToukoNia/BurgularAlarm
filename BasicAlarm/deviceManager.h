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
  public:
    void addSensor(int pinNo, int type);
    void removeSensor(int number);
    bool checkSensors();
    int getNumber();
};

class LockManager{
  private:
    int head=0; int tail=0; int i;
    Lock locks[MAX_IO];
  public:
    void addLock(int pinNo);
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