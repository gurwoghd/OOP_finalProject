#include <iostream>
#include <string>
#include <memory>
#include <fstream>
#include <sstream>
#include <vector>

#include "headers/LoginMenu.h"
#include "headers/User.h"

using namespace std;


LoginMenu::LoginMenu() {
    commands->push_back(make_shared<RegisterCommand>(new UserManager()));
    commands->push_back(make_shared<LoginasAdmin>(new UserManager()));
    commands->push_back(make_shared<LoginasCustomer>(new UserManager()));
}


void LoginMenu::display() {
    cout << "Welcome to the Book Recommendation System!" << endl;
    while (true) {
        int selection;
        cout << "1. Register" << endl;
        cout << "2. Login(Admin)" << endl;
        cout << "3. Login(Customer)" << endl;
        cout << "4. Exit" << endl;
        cout << "\nSelect an option: ";
        cin >> selection;

        if (selection >= commands->size()) {
            cout << "Invalid selection, select another one: ";
        }
        else if (selection == commands->size() - 1) // Exit ����� ���
        {
            break;
        }
        else {
            commands[selection - 1]->execute();
            break;
        }
    }
}


void RegisterCommand::execute() {
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

void LoginasAdmin::execute() {
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
        this->currentUser = make_shared<Admin>(adminID, adminPW);
    }
    else {
        cout << "ID or PW is incorrect." << endl;
    }
}

void LoginasCustomer::execute() {
    string id;
    string pw;
    cout << "Enter your ID: ";
    cin >> id;
    cout << "Enter your password: ";
    cin >> pw;
    if (manager.check(id, pw)) {
        cout << "Login successful, welcome " << id << endl;
        this->currentUser = make_shared<Customer>(id, pw);
    }
    else {
        cout << "ID or password is incorrect." << endl;
    }
}