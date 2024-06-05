#include <iostream>
#include <string>
#include <vector>
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

class UserManager {
private:
    vector<User> users;
public:
    void addNewUser(const string& id, const string& pw) {
        users.push_back(User(id, pw));
    }

    bool checkID(const string& id) const {
        for (const auto& user : users) {
            if (user.getID() == id) {
                return true;
            }
        }
        return false;
    }

    bool check(const string& id, const string& pw) const {
        for (const auto& user : users) {
            if (user.getID() == id && user.getPW() == pw) {
                return true;
            }
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
        Command* openLibrary = new OpenLibraryCommand();
        Command* purchaseBook = new PurchaseBookCommand();
        Command* getRecommendation = new GetRecommendationCommand();

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
    Command* registerCommand;
    Command* administerCommand;
    Command* customerCommand;
public:
    LoginMenu(Command* reg, Command* admin, Command* cus)
        : registerCommand(reg), administerCommand(admin), customerCommand(cus) {}

    void display() {
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

    Command* registerCommand = new RegisterCommand(manager);
    Command* administerCommand = new LoginasAdmin(manager);
    Command* customerCommand = new LoginasCustomer(manager);

    LoginMenu loginMenu(registerCommand, administerCommand, customerCommand);

    loginMenu.display();

    delete registerCommand;
    delete administerCommand;
    delete customerCommand;

    return 0;
}
