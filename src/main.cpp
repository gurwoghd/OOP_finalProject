#include <iostream>

#include "Command.h"
#include "User.h"
#include "LoginMenu.h"


int main() {
    UserManager manager;
    unique_ptr<User> currentUser;

    shared_ptr<Command> registerCommand = make_shared<RegisterCommand>(manager);
    shared_ptr<Command> administerCommand = make_shared<LoginAsAdmin>(manager);
    shared_ptr<Command> customerCommand = make_shared<LoginasCustomer>(manager);

    LoginMenu loginMenu(registerCommand, administerCommand, customerCommand);

    loginMenu.display(currentUser);

    return 0;
}
