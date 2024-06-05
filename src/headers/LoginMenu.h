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
        if (!manager.alreadyExistID(id)) {
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

class LoginasCustomer : public Command {
private:
    UserManager& manager;
    string currentUserID;
public:
    LoginasCustomer(UserManager& manager) : manager(manager) {}

    // Override execute from virtual class Command
    void execute() override {
        string id;
        string pw;
        cout << "Enter your ID: ";
        cin >> id;
        cout << "Enter your password: ";
        cin >> pw;
        if (manager.check(id, pw)) {
            cout << "Login successful, welcome " << id << endl;
            currentUserID = id;
            displayCustomerMenu();
        }
        else {
            cout << "ID or password is incorrect." << endl;
        }
    }
};

// Using the classes made above, the menu displays the login section
class LoginMenu {
private:
    shared_ptr<Command> registerCommand;
    shared_ptr<Command> administerCommand;
    shared_ptr<Command> customerCommand;
public:
    LoginMenu(shared_ptr<Command> reg, shared_ptr<Command> admin, shared_ptr<Command> cus)
        : registerCommand(reg), administerCommand(admin), customerCommand(cus) {}

    void display(unique_ptr<User> currentUser) {
        cout << "Welcome to the Book Recommendation System!" << endl;
        while (true) {
            int selection;
            cout << "1. Register" << endl;
            cout << "2. Login(Admin)" << endl;
            cout << "3. Login(Customer)" << endl;
            cout << "4. Exit" << endl;
            cout << "\nSelect an option: ";
            cin >> selection;

            switch (selection) {
            case 1:
                registerCommand->execute();
                break;
            case 2:
                administerCommand->execute();
                break;
            case 3:
                customerCommand->execute();
                break;
            case 4:
                return;
            default:
                cout << "Invalid selection, select another one: ";
            }
        }
    }
};

