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
#include "CustomerMenu.h"
#include "ErrorClasses.h"

using namespace std;
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
    
    static shared_ptr<BookManager> getBookManager() {return bookManager;}

private: 
    static shared_ptr<BookManager> bookManager;
    shared_ptr<AdminMenu> menu;

    friend class AdminMenu;
};

class Customer : public User {
public:
    Customer(const string& id, const string& pw) : User(id,pw), menu(make_unique<CustomerMenu>()) { bookRecommender = make_unique<BookRecommender>(); }
    
    virtual void showMenu() { this->menu->displayCommands(); }
    
    void addRecent();
    void showLibrary();

    static multimap<string, shared_ptr<Book>> getReadBookHistory() { return readBookHistory; }
    static multimap<string, shared_ptr<Book>> getPurchaseBookHistory() {return purchaseBookHistory;}
    static shared_ptr<BookRecommender> getBookRecommender() {return bookRecommender;}

private:
    static shared_ptr<BookRecommender> bookRecommender;
    unique_ptr<CustomerMenu> menu;
    
    fstream readBookDB;
    fstream purchaseBookDB;

    static multimap<string, shared_ptr<Book>> readBookHistory;
    static multimap<string, shared_ptr<Book>> purchaseBookHistory;
    
    friend class BookRecommender;
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
