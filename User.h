#pragma once

#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>


class AdminMenu;


class AdminMenuCommand {
public:
    AdminMenuCommand(shared_ptr<BookStorage> _bs) : bs(_bs) { }

    virtual void execute() = 0;
    virtual ~AdminMenuCommand() {}
protected:
    shared_ptr<BookStorage> bs;
};

class AdminMenu {
public:
    AdminMenu();

    void addCommand(shared_ptr<AdminMenuCommand> command);
    void displayCommands();
private:
    vector<shared_ptr<AdminMenuCommand>> commands;
    shared_ptr<BookStorage> bs;
};

class AddBook : public AdminMenuCommand {
public:
    AddBook(shared_ptr<BookStorage> _bs) : AdminMenuCommand(_bs) { }
    virtual void execute() override;
};

class DeleteBook : public AdminMenuCommand {
public:
    DeleteBook(shared_ptr<BookStorage> _bs) : AdminMenuCommand(_bs) { }

    void printBooks(string selectedGenre) const;
    virtual void execute() override;
};

//origin Book.h

class Book {
public:
    Book(string date, string title, string author, string publisher, string lang,
        float price, string genre)
        : date(date), title(title), author(author), publisher(publisher),
        lang(lang), price(price), genre(genre) {}
    void setPublisher(const string publisher) { this->publisher = publisher; }
    void setTitle(const string title) { this->title = title; }
    void setPrice(const float price) { this->price = price; }
    void setDate(const string date) { this->date = date; }
    void setAuthor(const string author) { this->author = author; }
    void setLang(const string lang) { this->lang = lang; }

    string getPublisher() const { return this->publisher; }
    string getTitle() const { return this->title; }
    float getPrice() const { return this->price; }
    string getDate() const { return this->date; }
    string getAuthor() const { return this->author; }
    string getLang() const { return this->lang; }
    string getGenre() const { return this->genre; }

    void viewInfo() const;

protected:
    string date;
    string title;
    string author;
    string publisher;
    string lang;
    float price;
    string genre;
    vector<string> detailGenre;

private:
    friend class BookManager;
    friend class BookRecommender; //오타 수정함//
};

// Genre별 constructor 추가했습니다.//
class Literature : public Book {
public:
    Literature(string date, string title, string author, string publisher,
        string lang, float price)
        : Book(date, title, author, publisher, lang, price, "Literature") {}
};

class Non_fiction : public Book {
public:
    Non_fiction(string date, string title, string author, string publisher,
        string lang, float price)
        : Book(date, title, author, publisher, lang, price, "Non_fiction") {}
};

class Practical : public Book {
public:
    Practical(string date, string title, string author, string publisher,
        string lang, float price)
        : Book(date, title, author, publisher, lang, price, "Practical") {}
};

class TeenAndChild : public Book {
public:
    TeenAndChild(string date, string title, string author, string publisher,
        string lang, float price)
        : Book(date, title, author, publisher, lang, price, "TeenAndChild") {}
};


//origin BookManager//
class BookManager {
public:
    BookManager();
    void addBook(Book& book, string genre);
    void deleteBook(string genre, string title);
private:
    multimap<string, unique_ptr<Book>> books;

    friend class Admin;
};


//origin BookRecommender

class BookRecommender {
public:
    void countGenre(vector<shared_ptr<Book>> history);
    void countAuthors(vector<shared_ptr<Book>> history);
    void countLang(vector<shared_ptr<Book>> history);
    void analyzeHistory(vector<shared_ptr<Book>> history);
    void makeRecommendation();
    void sortRecommendationByCount();
    void sortRecommendationByDate();
    void sortRecommendationByPrice();
    void sortRecommendation();
    void printRecommendation();
    void readBookHistory(shared_ptr<ostream> bookDatabase, vector<shared_ptr<Book>>& history);
private:
    multimap<string, unique_ptr<Book>> books;
    multimap<string, int> genreCount;
    multimap<string, int> authorCount;
    multimap<string, int> languageCount;
    multimap<string, int> detailGenre;
    multimap<string, unique_ptr<Book>> recommendResult;
};

class Author {
public:
    string name;
    int cnt;
};

class Language {
public:
    string lang;
    int cnt;
};

class DetailGenre {
public:
    string name;
    int cnt;
};

//origin bookstorage.h

class BookStorage {
public:
    BookStorage();

public:
    /////////////// 여기서 public으로 설정하는 거 괜춘?????
    multimap<string, shared_ptr<Book>> books;
    // shared_ptr<Customer> currentUser;
    fstream bookDB;
    array<string, 4> kindOfGenre = { "Literature", "Practical", "Non_fiction",
                                    "TeenAndChild" };
    string filepath = "../databases/BookDatabase.txt";
};

// Origin CustomerMenu.h//
class CustomerMenu;

class CustomerMenuCommand {
public:
    CustomerMenuCommand(shared_ptr<BookStorage> _bs) : bs(_bs) { }

    virtual void execute() = 0;
    void PrintBooks(string chosenGenre);
protected:
    shared_ptr<BookStorage> bs;
};


class CustomerMenu {
public:
    CustomerMenu();

    void addCommand(shared_ptr<CustomerMenuCommand> com);
    void displayCommands();
private:
    vector<shared_ptr<CustomerMenuCommand>> commands;
    shared_ptr<BookStorage> bs;
};

class OpenLibraryCommand : public CustomerMenuCommand {
public:
    OpenLibraryCommand(shared_ptr<BookStorage> _bs) : CustomerMenuCommand(_bs) { }
    virtual void execute() override;
    void PrintBookInfo(string chosenGenre, int BookNumber);
};

class PurchaseBookCommand : public CustomerMenuCommand {
public:
    PurchaseBookCommand(shared_ptr<BookStorage> _bs) : CustomerMenuCommand(_bs) { }

    virtual void execute() override;
    void purchase(string chosenGenre, int BookNumber);
};

class GetRecommendationCommand : public CustomerMenuCommand {
public:
    GetRecommendationCommand(shared_ptr<BookStorage> _bs) : CustomerMenuCommand(_bs) { }

    virtual void execute() override;
};

//origin LoginMenu.h//
// Virtual class Command

class CustomerMenu;

class CustomerMenuCommand {
public:
    CustomerMenuCommand(shared_ptr<BookStorage> _bs) : bs(_bs) { }

    virtual void execute() = 0;
    void PrintBooks(string chosenGenre);
protected:
    shared_ptr<BookStorage> bs;
};


class CustomerMenu {
public:
    CustomerMenu();

    void addCommand(shared_ptr<CustomerMenuCommand> com);
    void displayCommands();
private:
    vector<shared_ptr<CustomerMenuCommand>> commands;
    shared_ptr<BookStorage> bs;
};

class OpenLibraryCommand : public CustomerMenuCommand {
public:
    OpenLibraryCommand(shared_ptr<BookStorage> _bs) : CustomerMenuCommand(_bs) { }
    virtual void execute() override;
    void PrintBookInfo(string chosenGenre, int BookNumber);
};

class PurchaseBookCommand : public CustomerMenuCommand {
public:
    PurchaseBookCommand(shared_ptr<BookStorage> _bs) : CustomerMenuCommand(_bs) { }

    virtual void execute() override;
    void purchase(string chosenGenre, int BookNumber);
};

class GetRecommendationCommand : public CustomerMenuCommand {
public:
    GetRecommendationCommand(shared_ptr<BookStorage> _bs) : CustomerMenuCommand(_bs) { }

    virtual void execute() override;
};

class LoginMenuCommand {
public:
    virtual void execute() = 0;
    virtual ~LoginMenuCommand() {}
};

class LoginMenu {
private:
    unique_ptr<vector<shared_ptr<LoginMenuCommand>>> commands;
    bool logined;

public:
    shared_ptr<User> currentUser;

    LoginMenu();
    void addCommands(shared_ptr<LoginMenuCommand> command);
    void display() const;
    bool isLogined() const { return logined; }
    shared_ptr<User> getCurrentUser() const { return this->currentUser; }
};

// Register and login commands have inheritance from Command, polymorphism
class RegisterCommand : public LoginMenuCommand {
private:
    // UserManager is needed in order to add the newly made account into the user
    // vector
    unique_ptr<UserManager> manager;

public:
    RegisterCommand(unique_ptr<UserManager> manager) : manager(move(manager)) {}

    // Override execute from virtual class Command, polymorphism
    virtual void execute() override;
};

class LoginasAdmin : public LoginMenuCommand {
private:
    unique_ptr<UserManager> manager;
    shared_ptr<User> currentUser;

public:
    LoginasAdmin(unique_ptr<UserManager> manager) : manager(move(manager)) {}

    // Override execute from virtual class Command
    virtual void execute() override;
};

class LoginasCustomer : public LoginMenuCommand {
private:
    unique_ptr<UserManager> manager;
    shared_ptr<User> currentUser;

public:
    LoginasCustomer(unique_ptr<UserManager> manager) : manager(move(manager)) {}
    // Override execute from virtual class Command
    virtual void execute() override;
};


//origin ErrorClasses.h

class AlreadyExist : public exception {
public:
    virtual string what() { return "User already exsits\n"; }
};

class DatabaseNotOpen : public exception {
public:
    virtual string what() { return "File not opened\n"; }
};




// User.h
using namespace std;

class CustomerMenu;
class AdminMenu;

class User {
protected:
    string identification;
    string password;

public:
    User(const string& id, const string& pw) : identification(id), password(pw) {}

    string getID() const { return identification; }

    string getPW() const { return password; }

    virtual void showMenu() = 0;
};

class Admin : public User {
public:
    Admin(const string& id, const string& pw) : User(id, pw) {
        bookManager = make_shared<BookManager>();
        menu = make_shared<AdminMenu>();
    }

    virtual void showMenu() override { menu->displayCommands(); }

    static shared_ptr<BookManager> getBookManager() { return bookManager; }

private:
    static shared_ptr<BookManager> bookManager;
    shared_ptr<AdminMenu> menu;

    friend class AdminMenu;
};

class Customer : public User {
public:
    Customer(const string& id, const string& pw) : User(id, pw) {
        bookRecommender = make_unique<BookRecommender>();
        menu = make_shared<CustomerMenu>();
    }

    virtual void showMenu() { this->menu->displayCommands(); }

    void addRecent();
    void showLibrary();

    static multimap<string, shared_ptr<Book>> getReadBookHistory() {
        return readBookHistory;
    }
    static multimap<string, shared_ptr<Book>> getPurchaseBookHistory() {
        return purchaseBookHistory;
    }
    static shared_ptr<BookRecommender> getBookRecommender() {
        return bookRecommender;
    }

private:
    static shared_ptr<BookRecommender> bookRecommender;
    shared_ptr<CustomerMenu> menu;

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

    void addNewUser(const string& id, const string& pw) throw(AlreadyExist,
        DatabaseNotOpen);
    bool alreadyExistID(const string& id) const;
    bool check(const string& id, const string& pw) const;
};
