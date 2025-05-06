#include "users.h"

void User::updateCredentials(String ID, bool admin){  //change to constructor
  userID=ID;
  isAdmin=admin;
}
bool User::adminCheck(){  //returns if user is admin
  return isAdmin;
}

String User::getName(){ 
  return userID;
}

int AuthenticationManager::searchUsers(String UserID){ 
  for (i=0; i < head; i++) {  //searches the users to find the index given a set user ID, useful for managing data incoming from MATLAB
    if (users[i].getName()==UserID){
      return i;
    }
  }
  return -1;
}

int AuthenticationManager::authenticate(String userID, String pinAttempt){  //authenticate the user and return if authenticated, with the level of permits
  currentUser=searchUsers(userID);
  if (attemptNumber==maxAttempts){  //checks to see if exceeded max attempts
    return -1;
  }
  if (pin==pinAttempt&&(currentUser!=-1)){  //if user recognised and pin is correct, return 1 (or 2 if the user is an admin)
    attemptNumber=0;
    return (1+users[currentUser].adminCheck());
  }
  attemptNumber++;
 
  return 0;
}

bool AuthenticationManager::addUser(String userID, bool admin){ //adds a user
  if(head<MAX_USERS){
    users[head] = User();
    users[head].updateCredentials(userID, admin);  //same core logic as how the sensor and lock managers work
    head++;
    return 1;
  }
  return 0;
}

int AuthenticationManager::removeUser(String userID){ //removes a user, and outputs if failed bc last admin or if user cannot be found
  tail = searchUsers(userID);
  if (tail==currentUser){
    return -1;
  }
  else if (tail<0){
    return 0;
  }
  for (i = tail + 1; i < head; i++) {
    users[tail] = users[i];
    tail++;
  }
  head--;
  return 1;
}

void AuthenticationManager::updateMaxAttempts(int attempts){  //allows for updating the pin information to update the number of max attempts
  attemptNumber=0;
  maxAttempts=attempts;
}

void AuthenticationManager::updatePin(String newPin){ //updates pin
  pin=newPin;
}

void AuthenticationManager::resetAttempts(){  //resets attempts (for if locked out etc)
  attemptNumber=0;
}

void AuthenticationManager::printOut(){ //prints out the information needed to save to a file
  Serial.println("Credentials Start");
  while (!(Serial.available()>0));
  Serial.println(pin);
  Serial.println(maxAttempts);
  Serial.println("Credentials End");
  while (!(Serial.available()>0));
  Serial.println("User List Start");
  while (!(Serial.available()>0));
  for (i=0;i<head;i++){
    Serial.println(users[i].getName()+","+users[i].adminCheck());
  }
  Serial.println("User List End");
}