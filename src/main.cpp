#include <iostream>
#include <memory>

#include "headers/User.h"
#include "headers/LoginMenu.h"


int main() {
    UserManager manager;
    shared_ptr<User> currentUser;

    unique_ptr<LoginMenu> loginMenu = make_unique<LoginMenu>();

    loginMenu->display();

    
    if(loginMenu->isLogined()) { // if user is logined, show user's menu (admin or customer)
        currentUser = loginMenu->getCurrentUser();
        currentUser->showMenu();
    }

    return 0;
}
