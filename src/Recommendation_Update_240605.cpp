#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <stdexcept>
#include <memory>

#include "headers/BookManager.h"

using namespace std;

class User {
private:
    string identification;
    string password;
public:
    User(const string& id, const string& pw) : identification(id), password(pw) {}

    string getID() const {
        return identification;
    }

    string getPW() const {
        return password;
    }
};

class Admin {
public:

private: 
    unique_ptr<BookManager> bookManager;
    unique_ptr<AdminMenu> menu;

    friend class AdminMenu;
}



class AlreadyExist : public exception {
public:
    virtual string what() override{
        return "User already exsits\n";
    }
};

class DatabaseNotOpen : public exception {
public:
    virtual string what() override {
        return "File not opened\n";
    }
};

class UserManager {
private:
    // vector<User> users;
    fstream userDB;
    string filename;
public:
    UserManager() { string filename = "../databases/UserDatabase.txt"; }

    void addNewUser(const string& id, const string& pw) throw(AlreadyExist, DatabaseNotOpen) {
        // users.push_back(User(id, pw));
        User user(id, pw);
        userDB.open(filename);
        try {
            if(!userDB.is_open()) throw DatabaseNotOpen();
            if(alreadyExistID(id)) throw AlreadyExist();

            //////////////////////  사용자 추가 가능함   /////////////////////////////
            userDB << id << " " << pw << endl;
            userDB.close();
        } catch(DatabaseNotOpen& e) {
            cout << e.what() << endl;
        } catch(UserExit& e) {
            cout << e.what() << endl;
        }
        
    }

    // bool checkID(const string& id) const {
    //     for (const auto& user : users) {
    //         if (user.getID() == id) {
    //             return true;
    //         }
    //     }
    //     return false;
    // }

    bool alreadyExistID(const string& id) const {
        // userDatabase에 해당 아이디가 있으면 에러 발생 = 중복된 아이디 검사 기능
        string line;
        while(getline(userDB, line)){
            string token;
            stringstream ss(line);
            vector<string> a;
            while(getline(ss, token, " ")) {
                a.push_back(token);
            }
            // input ID equals ID in UserDatabase
            if(a[0] == id) return true;
        }
        return false;
    }

    // bool check(const string& id, const string& pw) const {
    //     for (const auto& user : users) {
    //         if (user.getID() == id && user.getPW() == pw) {
    //             return true;
    //         }
    //     }
    //     return false;
    // }

    bool check(const string& id, const string& pw) const {
        string line;
        while(getline(userDB, line)) {
            string token;
            stringstream ss(line);
            vector<string> a;
            while(getline(ss, token, " ")) a.push_back(token);
            
            if(a[0] == id && a[1] == pw) return true;
        }
        return false;
    }
};

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


class OpenLibraryCommand : public Command {
public:
    void execute() override {
        cout << "\nOpen Library selected.\n" << endl;
        // Add further implementation here
    }
};

class PurchaseBookCommand : public Command {
public:
    void execute() override {
        cout << "\nPurchase Book selected.\n" << endl;
        // Add further implementation here
    }
};

class GetRecommendationCommand : public Command {
public:
    void execute() override {
        cout << "\nGet Recommendation selected.\n" << endl;
        // Add further implementation here
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

    void displayCustomerMenu() {
        unique_ptr<Command> openLibrary = make_unique<OpenLibraryCommand>();
        unique_ptr<Command> purchaseBook = make_unique<PurchaseBookCommand>();
        unique_ptr<Command> getREcommendation = make_unique<GetRecommendationCommand>();

        while (true) {
            int selection;
            cout << "1. Open Library" << endl;
            cout << "2. Purchase Book" << endl;
            cout << "3. Get Recommendation" << endl;
            cout << "4. Logout" << endl;
            cout << "\nSelect an option: ";
            cin >> selection;

            switch (selection) {
            case 1:
                openLibrary->execute();
                break;
            case 2:
                purchaseBook->execute();
                break;
            case 3:
                getRecommendation->execute();
                break;
            case 4:
                delete openLibrary;
                delete purchaseBook;
                delete getRecommendation;
                return;
            default:
                cout << "Invalid selection, select another one: ";
            }
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
