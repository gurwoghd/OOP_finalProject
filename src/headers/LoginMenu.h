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
class LoginMenuCommand {
public:
    virtual void execute() = 0;
    virtual ~LoginMenuCommand() {}
};

class LoginMenu {
private:
    unique_ptr<vector<shared_ptr<LoginMenuCommand>>> commands;
    bool logined;    
public:
    shared_ptr<User> currentUser;
    
    LoginMenu();
    void addCommands(shared_ptr<LoginMenuCommand> command);
    void display() const;
    bool isLogined() const {return logined;}
    shared_ptr<User> getCurrentUser() const {return this->currentUser;}
};


// Register and login commands have inheritance from Command, polymorphism
class RegisterCommand : public LoginMenuCommand {
private:
    // UserManager is needed in order to add the newly made account into the user vector
    unique_ptr<UserManager> manager;
public:
    RegisterCommand(unique_ptr<UserManager> manager) : manager(move(manager)) {}

    // Override execute from virtual class Command, polymorphism
    virtual void execute() override;
};

class LoginasAdmin : public LoginMenuCommand {
private:
    unique_ptr<UserManager> manager;
    shared_ptr<User> currentUser;
public:
    LoginasAdmin(unique_ptr<UserManager> manager) : manager(move(manager)) {  }

    // Override execute from virtual class Command
    virtual void execute() override;
};

class LoginasCustomer : public LoginMenuCommand {
private:
    unique_ptr<UserManager> manager;
    shared_ptr<User> currentUser;
public:
    LoginasCustomer(unique_ptr<UserManager> manager) : manager(move(manager)) { }
    // Override execute from virtual class Command
    virtual void execute() override;
};
