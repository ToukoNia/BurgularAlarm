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

class AuthenticationManager{
  private:
    int maxAttempts;
    int attemptNumber=0;
    String pin;
    User users[MAX_USERS]; int head=0; int tail=0;
    int searchUsers(String UserID); int i; int currentUser;
  public:
    int authenticate(String userID, String pinAttempt);
    void updatePin(String newPin);
    void updateMaxAttempts(int attempts);
    bool addUser(String userID, bool isAdmin);
    int removeUser(String userID);
    void Setup(String newPin,int attempts, String userID);
};
#endif 
