#include <iostream>
#include <memory>
#include "Command.h"
#include "User.h"
#include "LoginMenu.h"


int main() {
    UserManager manager;
    shared_ptr<User> currentUser;

    LoginMenu loginMenu(currentUser);

    loginMenu.display();

    return 0;
}
