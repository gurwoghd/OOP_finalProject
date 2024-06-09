#include <iostream>
#include <string>
#include <memory>
#include <fstream>
#include <sstream>
#include <vector>

#include "headers/LoginMenu.h"
#include "headers/User.h"

using namespace std;

LoginMenu::LoginMenu() : currentUser(NULL), logined(false) {
    this->addCommands(make_shared<RegisterCommand>(make_unique<UserManager>()));
    this->addCommands(make_shared<LoginasAdmin>(make_unique<UserManager>()));
    this->addCommands(make_shared<LoginasCustomer>(make_unique<UserManager>()));
}

void LoginMenu::addCommands(shared_ptr<LoginMenuCommand> command) {
    commands->push_back(command);
}

void LoginMenu::display() const{
    cout << "Welcome to the Book Recommendation System!" << endl;
    while (true) {
        int selection;
        cout << "1. Register" << endl;
        cout << "2. Login(Admin)" << endl;
        cout << "3. Login(Customer)" << endl;
        cout << "4. Exit" << endl;
        cout << "\nSelect an option: ";
        cin >> selection;

        if (selection >= commands->size()) cout << "Invalid selection, select another one: ";
        else if (selection == commands->size() - 1) break;
        else {
            /////////////// 이렇게 짜면 register를 한 번 하면 프로그램이 종료됨. <- 이거 고치면 좋을 듯
            commands->at(selection - 1)->execute();
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
    if (!manager->alreadyExistID(id)) {
        manager->addNewUser(id, pw);
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
        this->currentUser = make_shared<Admin>(adminID, adminPW); // currentUser set
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
    if (manager->check(id, pw)) {
        cout << "Login successful, welcome " << id << endl;
        this->currentUser = make_shared<Customer>(id, pw); // currentUser set
    }
    else {
        cout << "ID or password is incorrect." << endl;
    }
}

// currentUser set은 single responsible에 어긋나는 것 같음. setCurrentUser를 함수 템플릿으로 만들면 좋을 것 같음
