#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <stdexcept>
#include <memory>

#include "headers/BookManager.h"

using namespace std;

//////////////////////  Exception clsses  ///////////////////////////////////
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

///////////////////////////////////////////////////////////////

class User {
protected:
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

class Admin : public User {
public:

private: 
    unique_ptr<BookManager> bookManager;
    unique_ptr<AdminMenu> menu;

    friend class AdminMenu;
}

class Customer : public User {
public:
    void addRecent();
    void showLibrary();
private:
    fstream readBookDB;
    fstream purchaseBookDB;
    
    friend class BookRecommender;
    friend class OpenLibrary;
    friend class PurchaseBook;
    friend class GetRecommendations;
}


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
