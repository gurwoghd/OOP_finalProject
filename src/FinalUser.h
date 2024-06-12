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


using namespace std;

//original User
class User {
protected:
    string identification;
    string password;

public:
    User(const string& id, const string& pw) : identification(id), password(pw) {}
    // ������
    virtual ~User() {}

    string getID() const { return identification; }

    string getPW() const { return password; }

    virtual void showMenu() = 0;
};

//original Errorclass
//////////////////////  Exception clsses  ///////////////////////////////////
class AlreadyExist : public exception {
public:
    // ������
    virtual string what() { return "User already exsits\n"; }
};

class DatabaseNotOpen : public exception {
public:
    // ������
    virtual string what() { return "File not opened\n"; }
};

///////////////////////////////////////////////////////////////



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


//original Book.h


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
    friend class BookRecommender;
};

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

//original BookManager.h
class BookManager {
public:
    BookManager();
    void addBook(Book& book, string genre);
    void deleteBook(string genre, string title);

private:
    multimap<string, Book*> books;

    friend class Admin;
};


//origin AdminMenu.h

//original Bookstorage.h
class BookStorage {
public:
    BookStorage();

public:
    /////////////// ���⼭ public���� �����ϴ� �� ����?????
    multimap<string, Book*> books;
    // shared_ptr<Customer> currentUser;
    fstream bookDB;
    array<string, 4> kindOfGenre = { "Literature", "Practical", "Non_fiction",
                                    "TeenAndChild" };
    string filepath = "../databases/BookDatabase.txt";

};

//original BookRecommender.h

class BookRecommender {
public:
    void countGenre(vector<Book*> history);
    void countAuthors(vector<Book*> history);
    void countLang(vector<Book*> history);
    void analyzeHistory(vector<Book*> history);
    void makeRecommendation();
    void sortRecommendationByCount();
    void sortRecommendationByDate();
    void sortRecommendationByPrice();
    void sortRecommendation();
    void printRecommendation();
    void readBookHistory(ostream* bookDatabase,
        vector<Book*>& history);

private:
    multimap<string, Book*> books;
    multimap<string, int> genreCount;
    multimap<string, int> authorCount;
    multimap<string, int> languageCount;
    multimap<string, int> detailGenre;
    multimap<string, Book*> recommendResult;
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

// original LoginMenu
// Virtual class Command
class LoginMenuCommand {
public:
    virtual void execute() = 0;
    virtual ~LoginMenuCommand() {}
};

class LoginMenu {
private:
    vector<LoginMenuCommand*> commands;
    bool logined;

protected:
    User* currentUser;
public:
    LoginMenu();
    void addCommands(LoginMenuCommand* command);
    void display() const;
    bool isLogined() const { return logined; }
    User* getCurrentUser() const { return this->currentUser; }
};

// Register and login commands have inheritance from Command, polymorphism
class RegisterCommand : public LoginMenuCommand {
private:
    // UserManager is needed in order to add the newly made account into the user
    // vector
    UserManager* manager;

public:
    RegisterCommand() : manager(new UserManager()) {}

    // Override execute from virtual class Command, polymorphism
    virtual void execute() override;
};

class LoginasAdmin : public LoginMenuCommand {
private:
    UserManager* manager;
    User* currentUser;

public:
    LoginasAdmin(User* _currentUser) : manager(new UserManager()), currentUser(_currentUser) {}

    // Override execute from virtual class Command
    virtual void execute() override;
};

class LoginasCustomer : public LoginMenuCommand {
private:
    UserManager* manager;
    User* currentUser;

public:
    LoginasCustomer(User* _currentUser) : manager(new UserManager()), currentUser(_currentUser) {}
    // Override execute from virtual class Command
    virtual void execute();
};



class Admin : public User {
public:
    Admin(const string& id, const string& pw) : User(id, pw) {
        menu = new AdminMenu();
    }
    // ������
    virtual ~Admin() {};

    // ������
    virtual void showMenu() override;

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
    AdminMenu* menu;

    friend class AdminMenu;
};

class Customer : public User {
public:
    Customer(const string& id, const string& pw) : User(id, pw) {
        menu = new CustomerMenu();
    }
    // ������ 
    virtual ~Customer() {}

    // ������
    virtual void showMenu();

    static multimap<string, Book*> getReadBookHistory() {
        return readBookHistory;
    }
    static multimap<string, Book*> getPurchaseBookHistory() {
        return purchaseBookHistory;
    }
    static BookRecommender* getBookRecommender() {
        return bookRecommender;
    }

public:
    class CustomerMenuCommand {
    public:
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
        // ������
        virtual ~OpenLibraryCommand() {};

        virtual void execute();
        void PrintBookInfo(string chosenGenre, int BookNumber);
    };

    class PurchaseBookCommand : public CustomerMenuCommand {
    public:
        PurchaseBookCommand(BookStorage* _bs) : CustomerMenuCommand(_bs) {}
        // ������
        virtual ~PurchaseBookCommand() {};

        virtual void execute() override;
        void purchase(string chosenGenre, int BookNumber);
    };

    class GetRecommendationCommand : public CustomerMenuCommand {
    public:
        GetRecommendationCommand(BookStorage* _bs)
            : CustomerMenuCommand(_bs) {}
        // ������
        virtual ~GetRecommendationCommand() {};

        virtual void execute() override;
    };

private:
    static BookRecommender* bookRecommender;
    CustomerMenu* menu;

    static multimap<string, Book*> readBookHistory;
    static multimap<string, Book*> purchaseBookHistory;
};
