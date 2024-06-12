#include <iostream>

#include "FinalUser.h"

using namespace std;

int main() {
    UserManager manager;
    User* currentUser;

    LoginMenu* loginMenu = new LoginMenu();

    loginMenu->display();
    if(loginMenu->isLogined()){
        std::cout << "Logined" << std::endl;
        currentUser = loginMenu->getCurrentUser();
        currentUser->showMenu();
    }else {
        std::cout << "Not Logined" << std::endl;
    }

}