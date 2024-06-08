#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <memory>
#include <map>

#include "BookManager.h"
#include "BookRecommender.h"
#include "AdminMenu.h"

using namespace std;

//////////////////////  Exception clsses  ///////////////////////////////////
class AlreadyExist : public exception {
public:
    virtual string what() {
        return "User already exsits\n";
    }
};

class DatabaseNotOpen : public exception {
public:
    virtual string what()  {
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

    string getID() const {return identification;}

    string getPW() const {return password;}

    virtual void showMenu() = 0;
};

class Admin : public User {
public:
    Admin(const string& id, const string& pw) : User(id, pw) { bookManager = make_shared<BookManager>(); }
    
    virtual void showMenu() override { menu->displayCommands(); }
    
    shared_ptr<BookManager> getBookManager() const {return bookManager;}

private: 
    shared_ptr<BookManager> bookManager;
    shared_ptr<AdminMenu> menu;

    friend class AdminMenu;
};

class Customer : public User {
public:
    Customer(const string& id, const string& pw) : User(id,pw) { bookRecommender = make_unique<BookRecommender>(); }
    
    virtual void showMenu() { menu->displayCommands(); }
    
    void addRecent();
    void showLibrary();

    multimap<string, shared_ptr<Book>> getReadBookHistory() const { return this->readBookHistory; }
    multimap<string, shared_ptr<Book>> getPurchaseBookHistory() const {return this->purchaseBookHistory;}
    shared_ptr<BookRecommender> getBookRecommender() const {return this->bookRecommender;}

    

private:
    shared_ptr<BookRecommender> bookRecommender;
    unique_ptr<CustomerMenu> menu;

    fstream readBookDB;
    fstream purchaseBookDB;

    multimap<string, shared_ptr<Book>> readBookHistory;
    multimap<string, shared_ptr<Book>> purchaseBookHistory;
    
    friend class BookRecommender;
    friend class OpenLibrary;
    friend class PurchaseBook;
    friend class GetRecommendations;
    friend class CustomerMenu;
};

class UserManager {
private:
    // vector<User> users;
    fstream userDB;
    string filename;
public:
    UserManager() { string filename = "../databases/UserDatabase.txt"; }

    void addNewUser(const string& id, const string& pw) throw(AlreadyExist, DatabaseNotOpen);
    bool alreadyExistID(const string& id) const;
    bool check(const string& id, const string& pw) const;
};
