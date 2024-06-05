#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>


#include "User.h"

using namespace std;

// Virtual class Command
class Command {
public:
    virtual void execute() = 0;
    virtual ~Command() {}
};

// Register and login commands have inheritance from Command, polymorphism
class RegisterCommand : public Command {
private:
    // UserManager is needed in order to add the newly made account into the user vector
    UserManager& manager;
public:
    RegisterCommand(UserManager& manager) : manager(manager) {}

    // Override execute from virtual class Command, polymorphism
    void execute() override {
        string id;
        string pw;
        cout << "Enter an ID to use: ";
        cin >> id;
        cout << "Enter a password to use: ";
        cin >> pw;
        if (!manager.checkID(id)) {
            manager.addNewUser(id, pw);
            cout << "\nRegister Complete! You are ready to log in!" << endl;
        }
        else {
            cout << "\nID already exists. Try again." << endl;
        }
    }
};

class LoginasAdmin : public Command {
private:
    UserManager& manager;
public:
    LoginasAdmin(UserManager& manager) : manager(manager) {}

    // Override execute from virtual class Command
    void execute() override {
        string id;
        string pw;
        string adminID = "admin";
        string adminPW = "admin1234";

        cout << "Enter the admin ID: ";
        cin >> id;
        cout << "Enter the admin PW: ";
        cin >> pw;
        if (id == adminID && pw == adminPW) {
            cout << "Welcome admin!" << endl;
        }
        else {
            cout << "ID or PW is incorrect." << endl;
        }
    }
};

