#include "systemController.h"
#include "users.h"
#define ALARM_DELAY 10
SystemController sys;
LockManager Locks;

String message;
SensorManager Sensors;
/*File Itenary
Device holds the code for the device class and those who inherit from it (sensors, locks, buzzer, led etc)
DeviceManager holds the code for the lockmanager, sensormanager and alarmmanager, which manage their selected arrays of compoments (including adding/removing devices etc)
SystemController has the SystemController object which contains the full system, and the SerialCommunicator to manage the communication with MATLAB
Users holds the authManager and user objects to allow for the authentication system
*/
int pinNo; bool logic; String name; int pinType;
void setup() {
  sys.setup();
  Serial.begin(9600);
  Locks.addLock(4,"Window");  //window
  Locks.addLock(2,"PIR Motion Detector");  //PIR
}

void loop() {
  //Locks.displayLockList(1);
  //sys.fullSystem();
  sys.loadSystem();
  
}

