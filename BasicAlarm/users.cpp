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
void UserManager::addUser(String userID, bool admin){
  users[head] = User();
  users[head].updateCredentials(userID, admin);  //change to constructors at somepoint
  head++;
}

int UserManager::searchUsers(String UserID){  //hashmap later?
  for (i=0; i < head; i++) {
    if (users[i].checkName(UserID)){
      return i;
    }
  }
  return -1;
}

void UserManager::removeUser(String userID) { //make have an error code for unfound value
  tail = searchUsers(userID);
  if (tail<0){
    return;
  }
  for (i = tail + 1; i < head; i++) {
    users[tail] = users[i];
    tail++;
  }
  head--;
}

int AuthenticationManager::authenticate(String userID, String pinAttempt){
  if (pin==pinAttempt){
    return (1+users.isAdmin(userID));
  }
  return 0;
}

void AuthenticationManager::addUser(String userID, bool isAdmin){
  users.addUser(userID, isAdmin);
}

void AuthenticationManager::removeUser(String userID){
  users.removeUser(userID);
}

void AuthenticationManager::updateAttempts(int attempts){
  attemptNumber=attempts;
}

void AuthenticationManager::updatePin(String newPin){
  pin=newPin;
}


