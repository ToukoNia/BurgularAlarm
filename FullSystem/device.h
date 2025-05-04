//device.h
#ifndef device_h
#define device_h
#include <Arduino.h>

class Device { 
  protected:
    int pinNumber;
    bool status;
    String name;
    int pinmode;
  public:
    void createDevice(int pinNo, bool currentStatus, int type, String ID);
    String getName();
    void printOut();
};

class Sensor: public Device {
  public:
    bool readFromSensor();
    void updateStatus(bool update);
    void printOut();
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