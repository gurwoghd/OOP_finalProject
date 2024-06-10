#include <iostream>
#include <string>
#include <memory>
#include <fstream>
#include <map>
#include <array>

#include "headers/User.h"
#include "headers/AdminMenu.h"
#include "headers/CustomerMenu.h"

class CustomerMenu;
class AdminMenu;

using namespace std;

void UserManager::addNewUser(const string& id, const string& pw) throw(AlreadyExist, DatabaseNotOpen) {
    // users.push_back(User(id, pw));
    unique_ptr<User> user = make_unique<Customer>(id, pw);
    userDB.open(filename);
    try {
        if (!userDB.is_open()) throw DatabaseNotOpen();
        if (alreadyExistID(user->getID())) throw AlreadyExist();

        //////////////////////  ����� �߰� ������   /////////////////////////////
        userDB << id << " " << pw << endl;
        userDB.close();
    }
    catch (DatabaseNotOpen& e) {
        cout << e.what() << endl;
    }
    catch (AlreadyExist& e) {
        cout << e.what() << endl;
    }

}

bool UserManager::alreadyExistID(const string& id) const {
    string line;
    fstream readUserDB;
    readUserDB.open(filename, fstream::in);
    while (getline(readUserDB, line)) {
        string token;
        stringstream ss(line);
        vector<string> a;
        while (getline(ss, token, ' ')) {
            a.push_back(token);
        }
        // input ID equals ID in UserDatabase
        if (a[0] == id) return true;
    }
    return false;
}
bool UserManager::check(const string& id, const string& pw) const {
    string line;
    fstream readUserDB;
    readUserDB.open(filename, fstream::in);
    while (getline(readUserDB, line)) {
        string token;
        stringstream ss(line);
        vector<string> a;
        while (getline(ss, token, ' ')) a.push_back(token);

        if (a[0] == id && a[1] == pw) return true;
    }
    return false;
}