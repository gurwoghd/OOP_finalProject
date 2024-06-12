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

#include "FinalUser.h"
using namespace std;

//origin AdminMenu.cpp

AdminMenu::AdminMenu() {
    bs = make_shared<BookStorage>();

    this->addCommand(make_shared<AddBook>(bs));
    this->addCommand(make_shared<DeleteBook>(bs));
}

void AdminMenu::addCommand(shared_ptr<AdminMenuCommand> command) {
    this->commands.push_back(command);
}

void AdminMenu::displayCommands() {
    int selection;
    cout << "Welcome Admin" << endl << endl;

    while (true) {
        cout << "What would you like to do?" << endl << endl;
        cout << "1. Add a book 2. Delete a book 3. Logout: ";
        cin >> selection;

        if (selection >= commands.size() || selection <= 0)
            cout << "Invalid selection, select another one: ";
        else if (selection == commands.size() - 1)
            break;
        else
            this->commands[selection - 1]->execute();
    }
}

void AddBook::execute() {
    int chosenGenre;
    array<string, 4> genres = { "Literature", "Practical", "Non-Fiction",
                               "Teen-and Child" };
    string title, publishDate, author, publisher, language, bookGenre;
    float price;
    try {
        bs->bookDB.open(bs->filepath);
        if (bs->bookDB.is_open())
            throw(DatabaseNotOpen());

        cout << "Select the Genre of the book to add" << endl << endl;
        cout << "1. Literature 2. Practical 3. Non-Fiction 4. Teen-and Child: ";
        cin >> chosenGenre;

        cout << "Add a " << genres[chosenGenre - 1] << " Book" << endl << endl;

        cout << "Enter the book name: ";
        cin >> title;
        cout << "Enter the published date: ";
        cin >> publishDate;
        cout << "Enter the price: ";
        cin >> price;
        cout << "Enter the author: ";
        cin >> author;
        cout << "Enter the publisher: ";
        cin >> publisher;
        cout << "Enter the language: ";
        cin >> language;
        cout << "Enter the genre: ";
        cin >> bookGenre;

        bs->books.insert(
            make_pair(bs->kindOfGenre[chosenGenre - 1],
                make_shared<Book>(publishDate, title, author, publisher,
                    language, price, bookGenre)));
        // bookDB에 업데이트
        bs->bookDB << publishDate << " " << title << " " << author << " "
            << publisher << " " << language << " " << to_string(price) << " "
            << bookGenre << endl;
        bs->bookDB.close();
    }
    catch (DatabaseNotOpen& e) {
        cout << e.what() << endl;
    }
}

void DeleteBook::execute() {
    string title; // title of book to delete
    array<string, 4> genres = { "Literature", "Practical", "Non-Fiction",
                               "Teen-and Child" };
    int chosenGenre;

    cout << "Select the Genre of the book to delete" << endl << endl;
    cout << "1. Literature 2. Practical 3. Non-Fiction 4. Teen-and Child : ";
    cin >> chosenGenre;

    this->printBooks(bs->kindOfGenre[chosenGenre - 1]);

    cout << "Enter the book name to delete: ";
    cin >> title;

    // 맵에서 제거
    auto rangeIter = bs->books.equal_range(bs->kindOfGenre[chosenGenre - 1]);
    for (auto it = rangeIter.first; it != rangeIter.second; ++it) {
        if (it->second->getTitle() == title) {
            bs->books.erase(it);
            break;
        }
    }
    // 데이터베이스에서 제거
    // 데이터베이스에서 title과 일치하는 제목을 포함하는 라인을 찾아서 그 라인만
    // 선택적으로 지워야 함
}

void DeleteBook::printBooks(string selectedGenre) const {
    auto rangeIter = bs->books.equal_range(selectedGenre);
    int idx = 1;
    for (auto it = rangeIter.first; it != rangeIter.second; ++it) {
        cout << idx << ". " << it->second->getTitle() << endl;
    }
}

//origin Book.cpp
void Book::viewInfo() const {
    cout << this->getTitle() << endl;
    cout << "Date: " << this->getDate() << endl;
    cout << "Price: " << this->getPrice() << endl;
    cout << "Author: " << this->getAuthor() << endl;
    cout << "Publisher: " << this->getPublisher() << endl;
    cout << "Language: " << this->getLang() << endl;
    cout << "Genre: " << this->getGenre() << endl;
}

//origin BookManager.cpp
BookManager::BookManager() {}

void BookManager::addBook(Book& book, string genre) {
    books.insert(make_pair(genre, make_unique<Book>(book)));
}

void BookManager::deleteBook(string genre, string title) {
    auto rangeIter = books.equal_range(genre);
    for (auto it = rangeIter.first; it != rangeIter.second; ++it) {
        // it->first는 key, second는 value이다.
        if (it->second->title == title) {
            books.erase(it);
            break;
        }
    }
}

//origin BookRecommender.cpp

void BookRecommender::countGenre(vector<shared_ptr<Book>> history) {
    this->genreCount.insert(make_pair("Literature", 0));
    this->genreCount.insert(make_pair("Non_fiction", 0));
    this->genreCount.insert(make_pair("Practical", 0));
    this->genreCount.insert(make_pair("TeenAndChild", 0));

    for (int i = 0; i < history.size(); ++i) {
        auto it = this->genreCount.find(history[i]->getGenre());
        it->second += 1;
    }
}

void BookRecommender::countAuthors(vector<shared_ptr<Book>> history) {

    for (int i = 0; i < history.size(); ++i) {
        auto it = authorCount.find(history[i]->getAuthor());
        if (it == authorCount.end()) // 저자가 없다면
            authorCount.insert(make_pair(history[i]->getAuthor(), 1));
        else
            it->second += 1;
    }
}
void BookRecommender::countLang(vector<shared_ptr<Book>> history) {

    for (int i = 0; i < history.size(); ++i) {
        auto it = authorCount.find(history[i]->getLang());
        if (it == authorCount.end()) // 저자가 없다면
            authorCount.insert(make_pair(history[i]->getLang(), 1));
        else
            it->second += 1;
    }
}
void BookRecommender::analyzeHistory(vector<shared_ptr<Book>> history) {}
void BookRecommender::makeRecommendation() {
    sortRecommendationByCount();
    sortRecommendationByDate();
    sortRecommendationByPrice();
}
void BookRecommender::sortRecommendationByCount() {}
void BookRecommender::sortRecommendationByDate() {}
void BookRecommender::sortRecommendationByPrice() {}
void BookRecommender::sortRecommendation() {}
void BookRecommender::printRecommendation() {
    array<string, 4> order = { "Literature", "Non_fiction", "Practical",
                              "TeenAndChild" };
    int cnt = 0;
    for (int i = 0; i < 4; i++) {
        cout << "**" << order[i] << " Recommendations\n";
        cnt = 0;
        auto rangeItr = recommendResult.equal_range(order[i]);
        for (auto it = rangeItr.first; it != rangeItr.second; ++it) {
            cout << cnt << ". " << it->second->getTitle() << endl;
        }
    }
}
void BookRecommender::readBookHistory(shared_ptr<ostream> bookDatabase,
    vector<shared_ptr<Book>>& history) {}

//origin BookStorage.cpp
BookStorage::BookStorage() {
    bookDB.open(filepath, fstream::in);

    try {
        // if not opened
        if (!bookDB.is_open())
            throw DatabaseNotOpen();
        // books 벡터 만들기
        string line;
        while (getline(bookDB, line)) {
            string token;
            stringstream ss(line);
            vector<string> a;
            while (getline(ss, token, ' ')) {
                a.push_back(token);
            }

            shared_ptr<Book> book;
            if (a[6] == "Literature")
                book =
                make_shared<Literature>(a[0], a[1], a[2], a[3], a[4], stof(a[5]));
            else if (a[6] == "Non_fiction")
                book =
                make_shared<Non_fiction>(a[0], a[1], a[2], a[3], a[4], stof(a[5]));
            else if (a[6] == "Practical")
                book = make_shared<Practical>(a[0], a[1], a[2], a[3], a[4], stof(a[5]));
            else if (a[6] == "TeenAndChild")
                book =
                make_shared<TeenAndChild>(a[0], a[1], a[2], a[3], a[4], stof(a[5]));
            books.insert(make_pair(a[6], book));
        }
        bookDB.close();
    }
    catch (DatabaseNotOpen& e) {
        cout << e.what() << endl;
    }
}

//origin CustomerMenu.cpp

CustomerMenu::CustomerMenu() {
    bs = make_shared<BookStorage>();
    // command를 추가함
    this->addCommand(make_shared<OpenLibraryCommand>(bs));
    this->addCommand(make_shared<PurchaseBookCommand>(bs));
    this->addCommand(make_shared<GetRecommendationCommand>(bs));
}

void CustomerMenu::addCommand(shared_ptr<CustomerMenuCommand> com) {
    commands.push_back(move(com));
}

void CustomerMenu::displayCommands() {
    int selection;
    while (true) {
        cout << "1. Open Library" << endl;
        cout << "2. Purchase Book" << endl;
        cout << "3. Get Recommendation" << endl;
        cout << "4. Logout" << endl;
        cout << "\nSelect an option: ";
        cin >> selection;

        if (selection >= commands.size() || selection <= 0)
            cout << "Invalid selection, select another one: ";
        else if (selection == commands.size() - 1)
            break;
        else
            this->commands[selection - 1]->execute();
    }
}

enum genre { LITERATURE = 1, PRACTICAL, NON_FICTION, TEEN_AND_CHILD };

void OpenLibraryCommand::execute() {
    int chosenGenre;
    // 수정함
    // short stay = 1;
    int BookNumber;

    cout << "\nOpen Library selected.\n" << endl;
    // Add further implementation here
    cout << "Choose a Genre\n";
    cout << "1.Literature 2. Practical 3. Non_fiction 4. Teen-and Child: ";
    cin >> chosenGenre;

    if (chosenGenre == genre::LITERATURE)
        PrintBooks("Literature");
    else if (chosenGenre == genre::PRACTICAL)
        PrintBooks("Practical");
    else if (chosenGenre == genre::NON_FICTION)
        PrintBooks("Non_fiction");
    else if (chosenGenre == genre::TEEN_AND_CHILD)
        PrintBooks("TeenAndChild");

    while (true) {
        cout << "Select a book to open, or enter 0 to exit the library: ";
        cin >> BookNumber;

        if (BookNumber == 0)
            break;
        else {
            this->PrintBookInfo(bs->kindOfGenre[chosenGenre - 1], BookNumber);
        }
    }
}

void CustomerMenuCommand::PrintBooks(string chosenGenre) {
    auto selectedGenreBooksIter = bs->books.equal_range(chosenGenre);
    int cnt = 1;
    cout << chosenGenre << endl;
    for (auto it = selectedGenreBooksIter.first;
        it != selectedGenreBooksIter.second; ++it) {
        cout << cnt << ". " << it->second->getTitle() << endl;
        cnt++;
    }
}

void OpenLibraryCommand::PrintBookInfo(string chosenGenre, int BookNumber) {
    auto BooksIter = bs->books.equal_range(chosenGenre);
    auto it = BooksIter.first;
    // 수정함
    for (int i = 0; i < BookNumber; i++)
        ++it;
    --it;
    auto selectedBook = it->second;
    selectedBook->viewInfo();

    // Customer의 readBookHistory에 추가
    Customer::getReadBookHistory().insert(make_pair(chosenGenre, selectedBook));
}

void PurchaseBookCommand::execute() {
    int chosenGenre;
    int BookNumber;
    short continuePurchase;

    cout << "\nPurchase Book selected.\n" << endl;
    // Add further implementation here
    cout << "Select the number of a Genre you are intrested" << endl;
    cout << "1. Literature 2. Practical 3. Non-fiction 4. Teen-and Child: ";
    cin >> chosenGenre;

    if (chosenGenre == LITERATURE)
        this->PrintBooks("Literature");
    else if (chosenGenre == PRACTICAL)
        this->PrintBooks("Practical");
    else if (chosenGenre == NON_FICTION)
        this->PrintBooks("Non_fiction");
    else if (chosenGenre == TEEN_AND_CHILD)
        this->PrintBooks("TeenAndChild");

    while (true) {
        cout << "Enter the number of the book you want to purchase: ";
        cin >> BookNumber;

        this->purchase(bs->kindOfGenre[chosenGenre - 1], BookNumber);
        cout << "1. Exit 2. Continue purchasing: ";
        cin >> continuePurchase;

        if (continuePurchase == 1) {
            break;
        }
        else if (continuePurchase == 2) {
            continue;
        }
    }
}

void PurchaseBookCommand::purchase(string chosenGenre, int BookNumber) {
    auto selectedBookIter = bs->books.equal_range(chosenGenre);
    auto it = selectedBookIter.first;
    // 수정함
    for (int i = 0; i < BookNumber; i++)
        ++it;
    --it;
    auto selectedBook = it->second;
    Customer::getPurchaseBookHistory().insert(
        make_pair(chosenGenre, selectedBook));

    cout << selectedBook->getTitle() << " has been successfully purchased"
        << endl;
}

void GetRecommendationCommand::execute() {
    cout << "\nGet Recommendation selected.\n" << endl;
    // Add further implementation here
    Customer::getBookRecommender()->printRecommendation();
}

//origin LoginMenu.cpp
//수정됨 (변수 순서 바꾸자..)
LoginMenu::LoginMenu() : logined(false), currentUser(nullptr) {
    this->addCommands(make_shared<RegisterCommand>(make_unique<UserManager>()));
    this->addCommands(make_shared<LoginasAdmin>(make_unique<UserManager>()));
    this->addCommands(make_shared<LoginasCustomer>(make_unique<UserManager>()));
}

void LoginMenu::addCommands(shared_ptr<LoginMenuCommand> command) {
    commands->push_back(command);
}

void LoginMenu::display() const {
    cout << "Welcome to the Book Recommendation System!" << endl;
    while (true) {
        int selection;
        cout << "1. Register" << endl;
        cout << "2. Login(Admin)" << endl;
        cout << "3. Login(Customer)" << endl;
        cout << "4. Exit" << endl;
        cout << "\nSelect an option: ";
        cin >> selection;

        if (selection >= commands->size())
            cout << "Invalid selection, select another one: ";
        else if (selection == commands->size() - 1)
            break;
        else {
            /////////////// 이렇게 짜면 register를 한 번 하면 프로그램이 종료됨. <-
            ///이거 고치면 좋을 듯
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

// currentUser set은 single responsible에 어긋나는 것 같음. setCurrentUser를
// 함수 템플릿으로 만들면 좋을 것 같음


class CustomerMenu;
class AdminMenu;
//Origin User.cpp
class User {
protected:
    string identification;
    string password;

public:
    User(const string& id, const string& pw) : identification(id), password(pw) {}
    // 수정함
    virtual ~User() {};

    string getID() const { return identification; }

    string getPW() const { return password; }

    virtual void showMenu() = 0;
};

class Admin : public User {
public:
    Admin(const string& id, const string& pw) : User(id, pw) {
        menu = make_shared<AdminMenu>();
    }
    // 수정함
    virtual ~Admin() {};

    // 수정함
    virtual void showMenu() override;

    static shared_ptr<BookManager> getBookManager() { return bookManager; }

private:
    static shared_ptr<BookManager> bookManager;
    shared_ptr<AdminMenu> menu;

    friend class AdminMenu;
};

class Customer : public User {
public:
    Customer(const string& id, const string& pw) : User(id, pw) {
        menu = make_shared<CustomerMenu>();
    }
    // 수정함 
    virtual ~Customer() {};

    // 수정함
    virtual void showMenu();

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