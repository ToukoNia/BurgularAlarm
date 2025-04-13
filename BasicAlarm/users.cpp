#include "users.h"

void User::updateCredentials(String ID, bool admin){  //change to constructor
  userID=ID;
  isAdmin=admin;
}
bool User::adminCheck(){
  return isAdmin;
}

bool User::checkName(String ID){
  if (ID==userID){
    return 1;
  }
  return 0;
}

int AuthenticationManager::searchUsers(String UserID){  //hashmap later?
  for (i=0; i < head; i++) {
    if (users[i].checkName(UserID)){
      return i;
    }
  }
  return -1;
}

int AuthenticationManager::authenticate(String userID, String pinAttempt){
  if (attemptNumber<maxAttempts){
    if (pin==pinAttempt){
      attemptNumber=0;
      return (1+users[searchUsers(userID)].adminCheck());
    }
    attemptNumber++;
    return 0;
  }
  return -1;
}

void AuthenticationManager::resetAttempts(){
  attemptNumber=0;
}

void AuthenticationManager::addUser(String userID, bool admin){
  users[head] = User();
  users[head].updateCredentials(userID, admin);  //change to constructors at somepoint
  head++;
}

bool AuthenticationManager::removeUser(String userID){
  tail = searchUsers(userID);
  if (tail<0){
    return 0;
  }
  for (i = tail + 1; i < head; i++) {
    users[tail] = users[i];
    tail++;
  }
  head--;
  return 1;
}

void AuthenticationManager::updateMaxAttempts(int attempts){
  maxAttempts=attempts;
}

void AuthenticationManager::updatePin(String newPin){
  pin=newPin;
}

void AuthenticationManager::Setup(String newPin,int attempts, String userID){
  addUser(userID,1);
  updateMaxAttempts(attempts);
  updatePin(newPin);
}

