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
#include <queue>

using namespace std;

//original User
class User {
protected:
    string identification;
    string password;

public:
    User(const string& id, const string& pw) : identification(id), password(pw) {}
    virtual ~User() {}

    string getID() const { return identification; }

    string getPW() const { return password; }

    virtual void showMenu() = 0;
};

//original Errorclass
//////////////////////  Exception clsses  ///////////////////////////////////
class AlreadyExist : public exception {
public:
    // which function made this error?
    AlreadyExist(string _functionName) : exception(), functionName(_functionName) { }
    virtual string what() { return "\nAt " + functionName + " : " + " ID already exists. Try Again\n"; }
private:
    string functionName;
};

class DatabaseNotOpen : public exception {
public:
    // which function made this error?
    DatabaseNotOpen(string _functionName) : exception(), functionName(_functionName) { }
    virtual string what() { return "\nAt " + functionName + " : " + " File not opened\n"; }
private:
    string functionName;
};

///////////////////////////////////////////////////////////////



class UserManager {
private:
    fstream userDB;
    string filepath;

public:
    UserManager(const string& _filepath = "databases/UserDatabase.txt") : filepath(_filepath) { }

    void addNewUser(const string& id, const string& pw) throw(AlreadyExist,
        DatabaseNotOpen);
    bool alreadyExistID(const string& id) const;
    bool check(const string& id, const string& pw) const throw(DatabaseNotOpen);
};


//original Book.h


class Book {
public:
    Book(string date, string title, string author, string publisher, string lang,
        float price, string genre, string detailGenre)
        : date(date), title(title), author(author), publisher(publisher),
        lang(lang), price(price), genre(genre), detailGenre(detailGenre) {}
    void setPublisher(const string publisher) { this->publisher = publisher; }
    void setTitle(const string title) { this->title = title; }
    void setPrice(const float price) { this->price = price; }
    void setDate(const string date) { this->date = date; }
    void setAuthor(const string author) { this->author = author; }
    void setLang(const string lang) { this->lang = lang; }
    void setDetailGenre(const string detailGenre) {this->detailGenre = detailGenre;}

    string getPublisher() const { return this->publisher; }
    string getTitle() const { return this->title; }
    float getPrice() const { return this->price; }
    string getDate() const { return this->date; }
    string getAuthor() const { return this->author; }
    string getLang() const { return this->lang; }
    string getGenre() const { return this->genre; }
    string getDetailGenre() const {return this->detailGenre;}

    // view info of books
    void viewInfo() const;

protected:
    string date;
    string title;
    string author;
    string publisher;
    string lang;
    float price;
    string genre;
    string detailGenre;

private:
    friend class BookManager;
    friend class BookRecommender;
};

class Literature : public Book {
public:
    Literature(string date, string title, string author, string publisher,
        string lang, float price, string detailGenre)
        : Book(date, title, author, publisher, lang, price, "Literature", detailGenre) {}
};

class Non_fiction : public Book {
public:
    Non_fiction(string date, string title, string author, string publisher,
        string lang, float price, string detailGenre)
        : Book(date, title, author, publisher, lang, price, "Non_fiction", detailGenre) {}
};

class Practical : public Book {
public:
    Practical(string date, string title, string author, string publisher,
        string lang, float price, string detailGenre)
        : Book(date, title, author, publisher, lang, price, "Practical", detailGenre) {}
};

class TeenAndChild : public Book {
public:
    TeenAndChild(string date, string title, string author, string publisher,
        string lang, float price, string detailGenre)
        : Book(date, title, author, publisher, lang, price, "TeenAndChild", detailGenre) {}
};

//original BookManager.h
class BookManager {
public:
    BookManager();
    void addBook(Book& book, string genre);
    void deleteBook(string genre, string title);

private:
    // use map using Entry (key-value)
    multimap<string, Book*> books;

    friend class Admin;
};

// class about book
// 
class BookStorage {
public:
    BookStorage();

public:
    multimap<string, Book*> books; // books in databases according to each book genre
    fstream bookDB;
    array<string, 4> kindOfGenre = { "Literature", "Practical", "Non_fiction",
                                    "TeenAndChild" };
    string filepath = "databases/BookDatabase.txt";

};

// Primary Class for recommending books
class BookRecommender {
public:
    void makeHistoryList(); // add 10 purchased books + 10 accessed books to history attribute
    void printRecomentation();
    void makeRecommendation(); // make recommendation list
    void sortRecommendationByCount();
    void countDetailGenre(); // count how much and which detail genres are accessed and purchased by customers

    void findMostCount(); // Find the most accessed and purchased detail genre in Literature, practical, non-fiction, and teen and child
    void printRecommendation(); // print recommend result


public:
    // utility functions for makeHistoryList() and countDetailGenre()
    void add(multimap<string, Book*> source, string g);
    void count( string g); // 세부 장르의 개수 카운트

private:
    multimap<string, Book*> history;
    array<priority_queue<Book*>, 4> recommendResult; // 추천 결과
    multimap<string, int> DGCount;
    vector<string> mostCount;
};

class LoginMenuCommand {
public:
    virtual void execute() = 0;
    virtual ~LoginMenuCommand() {}
};

class LoginMenu {
private:
    vector<LoginMenuCommand*> commands;
    
public:
    static bool logined;
    static User* currentUser;
public:
    LoginMenu();
    void addCommands(LoginMenuCommand* command);
    void display() const;
    bool isLogined() const { return logined; }
    User* getCurrentUser() const { return LoginMenu::currentUser; }
};

// Register and login commands have inheritance from LoginMenuCommand, polymorphism
class RegisterCommand : public LoginMenuCommand {
private:
    unique_ptr<UserManager> manager;

public:
    RegisterCommand() : manager(make_unique<UserManager>()) {}

    virtual void execute();
};

class LoginasAdmin : public LoginMenuCommand {
private:
    unique_ptr<UserManager> manager;
    User* currentUser;

public:
    LoginasAdmin() : manager(make_unique<UserManager>()) { }

    virtual void execute() override;
};

class LoginasCustomer : public LoginMenuCommand {
private:
    unique_ptr<UserManager> manager;

public:
    LoginasCustomer() : manager(make_unique<UserManager>()) { }
    virtual void execute();
};



class Admin : public User {
public:
    Admin(const string& id, const string& pw) : User(id, pw) {
        menu = make_unique<AdminMenu>();
    }
    virtual ~Admin() {};
    virtual void showMenu() override {menu->displayCommands();}

    BookManager* getBookManager() { return bookManager; }

public:
    class AdminMenuCommand {
    public:
        AdminMenuCommand(BookStorage* _bs) : bs(_bs) {}

        virtual void execute() = 0;
        virtual ~AdminMenuCommand() {}

    protected:
        BookStorage* bs;
    };

    class AdminMenu {
    public:
        AdminMenu();

        void addCommand(AdminMenuCommand* command);
        void displayCommands();

    private:
        vector<AdminMenuCommand*> commands;
        BookStorage* bs;
    };

    class AddBook : public AdminMenuCommand {
    public:
        AddBook(BookStorage* _bs) : AdminMenuCommand(_bs) {}
        virtual void execute();
    };

    class DeleteBook : public AdminMenuCommand {
    public:
        DeleteBook(BookStorage* _bs) : AdminMenuCommand(_bs) {}

        void printBooks(string selectedGenre) const;
        virtual void execute();
    };


private:
    BookManager* bookManager;
    unique_ptr<AdminMenu> menu;

    friend class AdminMenu;
};

class Customer : public User {
public:
    Customer(const string& id, const string& pw) : User(id, pw) {
        menu = make_unique<CustomerMenu>();
    }
    virtual ~Customer() {}

    virtual void showMenu() override {menu->displayCommands();}

    static multimap<string, Book*> getReadBookHistory() {
        return Customer::accessedBooks;
    }
    static multimap<string, Book*> getPurchaseBookHistory() {
        return Customer::purchasedBooks;
    }
    static BookRecommender* getBookRecommender() {
        return Customer::bookRecommender;
    }

public:
    class CustomerMenuCommand {
    public:
        // Each MenuCommand commonly have BookStorage bs since it serves some useful info
        CustomerMenuCommand(BookStorage* _bs) : bs(_bs) {}
        virtual ~CustomerMenuCommand() { }
        virtual void execute() = 0;
        void PrintBooks(string chosenGenre);

    protected:
        BookStorage* bs;
    };

    class CustomerMenu {
    public:
        CustomerMenu();

        void addCommand(CustomerMenuCommand* com);
        void displayCommands();

    private:
        vector<CustomerMenuCommand*> commands;
        BookStorage* bs;
    };

    class OpenLibraryCommand : public CustomerMenuCommand {
    public:
        OpenLibraryCommand(BookStorage* _bs) : CustomerMenuCommand(_bs) {}
        virtual ~OpenLibraryCommand() {};

        virtual void execute();
        void PrintBookInfo(string chosenGenre, int BookNumber);
    private:
        bool empty(string chosenGenre) const { 
            auto selectedBookIter = bs->books.equal_range(chosenGenre);
            return selectedBookIter.first == selectedBookIter.second;
        }
    };

    class PurchaseBookCommand : public CustomerMenuCommand {
    public:
        PurchaseBookCommand(BookStorage* _bs) : CustomerMenuCommand(_bs) {}
        virtual ~PurchaseBookCommand() {};

        virtual void execute() override;
        void purchase(string chosenGenre, int BookNumber);
    private:
        bool empty(string chosenGenre) const { 
            auto selectedBookIter = bs->books.equal_range(chosenGenre);
            return selectedBookIter.first == selectedBookIter.second;
        }
    };

    class GetRecommendationCommand : public CustomerMenuCommand {
    public:
        GetRecommendationCommand(BookStorage* _bs)
            : CustomerMenuCommand(_bs) {}
        virtual ~GetRecommendationCommand() {};

        virtual void execute() override;
    private:
        bool empty(string chosenGenre) const { 
            auto selectedBookIter = bs->books.equal_range(chosenGenre);
            return selectedBookIter.first == selectedBookIter.second;
        }
    };

private:
    unique_ptr<CustomerMenu> menu;
public: 
    static BookRecommender* bookRecommender;
    static multimap<string, Book*> accessedBooks;
    static multimap<string, Book*> purchasedBooks;
};
