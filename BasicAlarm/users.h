#ifndef users_h
#define users_h
#include <Arduino.h>
#define MAX_USERS 12
class User { 
  private:
    String userID;
    bool isAdmin;
  public:
    bool adminCheck();
    bool checkName(String ID);
    void updateCredentials(String ID, bool admin);
};

class UserManager { 
  private:
    User users[MAX_USERS]; int head=0; int tail=0;
    int searchUsers(String UserID); int i; 
  public:
    void addUser(String userID, bool admin);
    void removeUser(String userID);
    bool isAdmin(String userID);
};

class AuthenticationManager{
  private:
    int maxAttempts;
    int lockoutAttempts;
    int attemptNumber;
    String pin;
    UserManager users;
  public:
    int authenticate(String userID, String pinAttempt);
    void updatePin(String newPin);
    void updateAttempts(int attempts);
    void addUser(String userID, bool isAdmin);
    void removeUser(String userID);
   // void updateUser(String userID);
};
#endif 
