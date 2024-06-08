#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include "User.h"
#include "CustomerMenu.h"

using namespace std;

// Virtual class Command
class Command {
public:
    virtual void execute() = 0;
    virtual ~Command() {}
};

class LoginMenu {
private:
    unique_ptr<vector<Command>> commands;
        
public:
    shared_ptr<User> currentUser;

    LoginMenu(shared_ptr<User> _user) : currentUser(_user) { }
    void display();
};


// Register and login commands have inheritance from Command, polymorphism
class RegisterCommand : public Command {
private:
    // UserManager is needed in order to add the newly made account into the user vector
    UserManager& manager;
public:
    RegisterCommand(UserManager& manager) : manager(manager) {}

    // Override execute from virtual class Command, polymorphism
    void execute() override;
};

class LoginasAdmin : public Command {
private:
    UserManager& manager;
    shared_ptr<User> currentUser;
public:
    LoginasAdmin(UserManager& manager) : manager(manager) {  }

    // Override execute from virtual class Command
    void execute() override;
};

class LoginasCustomer : public Command {
private:
    UserManager& manager;
    shared_ptr<User> currentUser;
public:
    LoginasCustomer(UserManager& manager) : manager(manager) { }
    // Override execute from virtual class Command
    void execute() override;
};
