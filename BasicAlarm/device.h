//device.h
#ifndef device_h
#define device_h
#include <Arduino.h>

class Device { 
  protected:
    int pinNumber;
    bool status;
    String name;
  public:
    void createDevice(int pinNo, bool currentStatus, int type, String ID);
    bool getStatus();
    String getName();
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
  public:
    void changeState();
    void turnOff();
};

#endif