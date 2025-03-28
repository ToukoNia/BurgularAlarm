//device.h
#ifndef device_h
#define device_h
#include <Arduino.h>

class Device { 
  protected:
    int pinNumber;
    bool status;
   // char[] name;
  public:
    void createDevice(int pinNo, bool currentStatus, int type);
    bool getStatus();
};

class Sensor: public Device {
  public:
    bool readFromSensor();
    void updateStatus(bool update);
};

class Lock: public Device {
  public:
    void lock();
    void unlock();
};

class Buzzer: public Device {
  public:
    void sound();
    void stop();
};

class LED: public Device{  
  private:
    void turnOn();
    void turnOff();
  public:
    void changeState();
};

#endif