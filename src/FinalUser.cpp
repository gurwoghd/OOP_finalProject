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

Admin::AdminMenu::AdminMenu() {
    bs = new BookStorage();

    this->addCommand(new AddBook(bs));
    this->addCommand(new DeleteBook(bs));
}

void Admin::AdminMenu::addCommand(AdminMenuCommand* command) {
    this->commands.push_back(command);
}

void Admin::AdminMenu::displayCommands() {
    int selection;
    std::cout << "Welcome Admin" << endl << endl;

    while (true) {
        std::cout << "What would you like to do?" << endl << endl;
        std::cout << "1. Add a book 2. Delete a book 3. Logout: ";
        std::cin >> selection;

        if (selection >= commands.size() || selection <= 0)
            std::cout << "Invalid selection, select another one: ";
        else if (selection == commands.size() - 1)
            break;
        else
            this->commands[selection - 1]->execute();
    }
}

void Admin::AddBook::execute() {
    int chosenGenre;
    array<string, 4> genres = { "Literature", "Practical", "Non-Fiction",
                               "Teen-and Child" };
    string title, publishDate, author, publisher, language, bookGenre;
    float price;
    try {
        bs->bookDB.open(bs->filepath);
        if (bs->bookDB.is_open())
            throw(DatabaseNotOpen());

        std::cout << "Select the Genre of the book to add" << endl << endl;
        std::cout << "1. Literature 2. Practical 3. Non-Fiction 4. Teen-and Child: ";
        std::cin >> chosenGenre;

        std::cout << "Add a " << genres[chosenGenre - 1] << " Book" << endl << endl;

        std::cout << "Enter the book name: ";
        std::cin >> title;
        std::cout << "Enter the published date: ";
        std::cin >> publishDate;
        std::cout << "Enter the price: ";
        std::cin >> price;
        std::cout << "Enter the author: ";
        std::cin >> author;
        std::cout << "Enter the publisher: ";
        std::cin >> publisher;
        std::cout << "Enter the language: ";
        std::cin >> language;
        std::cout << "Enter the genre: ";
        std::cin >> bookGenre;

        bs->books.insert(
            make_pair(bs->kindOfGenre[chosenGenre - 1],
                new Book(publishDate, title, author, publisher,
                    language, price, bookGenre)));
        // bookDB�� ������Ʈ
        bs->bookDB << publishDate << " " << title << " " << author << " "
            << publisher << " " << language << " " << to_string(price) << " "
            << bookGenre << endl;
        bs->bookDB.close();
    }
    catch (DatabaseNotOpen& e) {
        std::cout << e.what() << endl;
    }
}

void Admin::DeleteBook::execute() {
    string title; // title of book to delete
    array<string, 4> genres = { "Literature", "Practical", "Non-Fiction",
                               "Teen-and Child" };
    int chosenGenre;

    std::cout << "Select the Genre of the book to delete" << endl << endl;
    std::cout << "1. Literature 2. Practical 3. Non-Fiction 4. Teen-and Child : ";
    std::cin >> chosenGenre;

    this->printBooks(bs->kindOfGenre[chosenGenre - 1]);

    std::cout << "Enter the book name to delete: ";
    std::cin >> title;

    // �ʿ��� ����
    auto rangeIter = bs->books.equal_range(bs->kindOfGenre[chosenGenre - 1]);
    for (auto it = rangeIter.first; it != rangeIter.second; ++it) {
        if (it->second->getTitle() == title) {
            bs->books.erase(it);
            break;
        }
    }
    // �����ͺ��̽����� ����
    // �����ͺ��̽����� title�� ��ġ�ϴ� ������ �����ϴ� ������ ã�Ƽ� �� ���θ�
    // ���������� ������ ��
}

void Admin::DeleteBook::printBooks(string selectedGenre) const {
    auto rangeIter = bs->books.equal_range(selectedGenre);
    int idx = 1;
    for (auto it = rangeIter.first; it != rangeIter.second; ++it) {
        std::cout << idx << ". " << it->second->getTitle() << endl;
    }
}

//origin Book.cpp
void Book::viewInfo() const {
    std::cout << this->getTitle() << endl;
    std::cout << "Date: " << this->getDate() << endl;
    std::cout << "Price: " << this->getPrice() << endl;
    std::cout << "Author: " << this->getAuthor() << endl;
    std::cout << "Publisher: " << this->getPublisher() << endl;
    std::cout << "Language: " << this->getLang() << endl;
    std::cout << "Genre: " << this->getGenre() << endl;
}

//origin BookManager.cpp
BookManager::BookManager() : books() {}

void BookManager::addBook(Book& book, string genre) {
    books.insert(make_pair(genre, new Book(book)));
}

void BookManager::deleteBook(string genre, string title) {
    auto rangeIter = books.equal_range(genre);
    for (auto it = rangeIter.first; it != rangeIter.second; ++it) {
        // it->first�� key, second�� value�̴�.
        if (it->second->title == title) {
            books.erase(it);
            break;
        }
    }
}


//origin BookStorage.cpp
BookStorage::BookStorage() {
    bookDB.open(filepath, fstream::in);

    try {
        // if not opened
        if (!bookDB.is_open())
            throw DatabaseNotOpen();
        // books ���� �����
        string line;
        while (getline(bookDB, line)) {
            string token;
            stringstream ss(line);
            vector<string> a;
            while (getline(ss, token, ' ')) {
                a.push_back(token);
            }

            Book* book;
            if (a[6] == "Literature")
                book =
                new (Literature)(a[0], a[1], a[2], a[3], a[4], stof(a[5]));
            else if (a[6] == "Non_fiction")
                book =
                new (Non_fiction)(a[0], a[1], a[2], a[3], a[4], stof(a[5]));
            else if (a[6] == "Practical")
                book = new (Practical)(a[0], a[1], a[2], a[3], a[4], stof(a[5]));
            else if (a[6] == "TeenAndChild")
                book =
                new (TeenAndChild)(a[0], a[1], a[2], a[3], a[4], stof(a[5]));
            books.insert(make_pair(a[6], book));
        }
        bookDB.close();
    }
    catch (DatabaseNotOpen& e) {
        std::cout << e.what() << endl;
    }
}

//origin CustomerMenu.cpp

Customer::CustomerMenu::CustomerMenu() {
    bs = new BookStorage();
    // command�� �߰���
    this->addCommand(new OpenLibraryCommand(bs));
    this->addCommand(new PurchaseBookCommand(bs));
    this->addCommand(new GetRecommendationCommand(bs));
}

void Customer::CustomerMenu::addCommand(CustomerMenuCommand* com) {
    commands.push_back(com);
}

void Customer::CustomerMenu::displayCommands() {
    int selection;
    while (true) {
        std::cout << "1. Open Library" << endl;
        std::cout << "2. Purchase Book" << endl;
        std::cout << "3. Get Recommendation" << endl;
        std::cout << "4. Logout" << endl;
        std::cout << "\nSelect an option: ";
        std::cin >> selection;

        if (selection >= commands.size() || selection <= 0)
            std::cout << "Invalid selection, select another one: ";
        else if (selection == commands.size() - 1)
            break;
        else
            this->commands[selection - 1]->execute();
    }
}

enum genre { LITERATURE = 1, PRACTICAL, NON_FICTION, TEEN_AND_CHILD };

void Customer::OpenLibraryCommand::execute() {
    int chosenGenre;
    // ������
    // short stay = 1;
    int BookNumber;

    std::cout << "\nOpen Library selected.\n" << endl;
    // Add further implementation here
    std::cout << "Choose a Genre\n";
    std::cout << "1.Literature 2. Practical 3. Non_fiction 4. Teen-and Child: ";
    std::cin >> chosenGenre;

    if (chosenGenre == genre::LITERATURE)
        PrintBooks("Literature");
    else if (chosenGenre == genre::PRACTICAL)
        PrintBooks("Practical");
    else if (chosenGenre == genre::NON_FICTION)
        PrintBooks("Non_fiction");
    else if (chosenGenre == genre::TEEN_AND_CHILD)
        PrintBooks("TeenAndChild");

    while (true) {
        std::cout << "Select a book to open, or enter 0 to exit the library: ";
        std::cin >> BookNumber;

        if (BookNumber == 0)
            break;
        else {
            this->PrintBookInfo(bs->kindOfGenre[chosenGenre - 1], BookNumber);
        }
    }
}

void Customer::CustomerMenuCommand::PrintBooks(string chosenGenre) {
    auto selectedGenreBooksIter = bs->books.equal_range(chosenGenre);
    int cnt = 1;
    std::cout << chosenGenre << endl;
    for (auto it = selectedGenreBooksIter.first;
        it != selectedGenreBooksIter.second; ++it) {
        std::cout << cnt << ". " << it->second->getTitle() << endl;
        cnt++;
    }
}

typedef multimap<string, Book*> (*f)();
void Customer::OpenLibraryCommand::PrintBookInfo(string chosenGenre, int BookNumber) {
    auto BooksIter = bs->books.equal_range(chosenGenre);
    auto it = BooksIter.first;
    // ������
    for (int i = 0; i < BookNumber; i++)
        ++it;
    --it;
    auto selectedBook = it->second;
    selectedBook->viewInfo();

    // Customer�� readBookHistory�� �߰�
    Customer::getReadBookHistory().insert(make_pair(chosenGenre, selectedBook));
}

void Customer::PurchaseBookCommand::execute() {
    int chosenGenre;
    int BookNumber;
    short continuePurchase;

    std::cout << "\nPurchase Book selected.\n" << endl;
    // Add further implementation here
    std::cout << "Select the number of a Genre you are intrested" << endl;
    std::cout << "1. Literature 2. Practical 3. Non-fiction 4. Teen-and Child: ";
    std::cin >> chosenGenre;

    if (chosenGenre == LITERATURE)
        this->PrintBooks("Literature");
    else if (chosenGenre == PRACTICAL)
        this->PrintBooks("Practical");
    else if (chosenGenre == NON_FICTION)
        this->PrintBooks("Non_fiction");
    else if (chosenGenre == TEEN_AND_CHILD)
        this->PrintBooks("TeenAndChild");

    while (true) {
        std::cout << "Enter the number of the book you want to purchase: ";
        std::cin >> BookNumber;

        this->purchase(bs->kindOfGenre[chosenGenre - 1], BookNumber);
        std::cout << "1. Exit 2. Continue purchasing: ";
        std::cin >> continuePurchase;

        if (continuePurchase == 1) {
            break;
        }
        else if (continuePurchase == 2) {
            continue;
        }
    }
}

typedef multimap<string, Book*> (*f)();
void Customer::PurchaseBookCommand::purchase(string chosenGenre, int BookNumber) {
    auto selectedBookIter = bs->books.equal_range(chosenGenre);
    auto it = selectedBookIter.first;
    // ������
    for (int i = 0; i < BookNumber; i++)
        ++it;
    --it;
    auto selectedBook = it->second;
    Customer::getPurchaseBookHistory().insert(
        make_pair(chosenGenre, selectedBook));

    std::cout << selectedBook->getTitle() << " has been successfully purchased"
        << endl;
}

void Customer::GetRecommendationCommand::execute() {
    std::cout << "\nGet Recommendation selected.\n" << endl;
    // Add further implementation here
    Customer::getBookRecommender()->printRecommendation();
}

//origin LoginMenu.cpp
//������ (���� ���� �ٲ���..)
LoginMenu::LoginMenu() : logined(false), currentUser(nullptr) {
    this->addCommands(new RegisterCommand());
    this->addCommands(new LoginasAdmin(currentUser));
    this->addCommands(new LoginasCustomer(currentUser));
}

void LoginMenu::addCommands(LoginMenuCommand* command) {
    commands.push_back(command);
}

void LoginMenu::display() const {
    std::cout << "Welcome to the Book Recommendation System!" << endl;
    while (true) {
        int selection;
        std::cout << "1. Register" << endl;
        std::cout << "2. Login(Admin)" << endl;
        std::cout << "3. Login(Customer)" << endl;
        std::cout << "4. Exit" << endl;
        std::cout << "\nSelect an option: ";
        std::cin >> selection;

        if (selection >= commands.size())
            std::cout << "Invalid selection, select another one: ";
        else if (selection == commands.size() - 1)
            break;
        else {
            commands[(selection - 1)]->execute();
            break;
        }
    }
}

void RegisterCommand::execute() {
    string id;
    string pw;
    std::cout << "Enter an ID to use: ";
    std::cin >> id;
    std::cout << "Enter a password to use: ";
    std::cin >> pw;
    if (!manager->alreadyExistID(id)) {
        manager->addNewUser(id, pw);
        std::cout << "\nRegister Complete! You are ready to log in!" << endl;
    }
    else {
        std::cout << "\nID already exists. Try again." << endl;
    }
}

void LoginasAdmin::execute() {
    string id;
    string pw;
    string adminID = "admin";
    string adminPW = "admin1234";

    std::cout << "Enter the admin ID: ";
    std::cin >> id;
    std::cout << "Enter the admin PW: ";
    std::cin >> pw;
    if (id == adminID && pw == adminPW) {
        std::cout << "Welcome admin!" << endl;
        this->currentUser = new Admin(adminID, adminPW); // currentUser set
    }
    else {
        std::cout << "ID or PW is incorrect." << endl;
    }
}

void LoginasCustomer::execute() {
    string id;
    string pw;
    std::cout << "Enter your ID: ";
    std::cin >> id;
    std::cout << "Enter your password: ";
    std::cin >> pw;
    if (manager->check(id, pw)) {
        std::cout << "Login successful, welcome " << id << endl;
        this->currentUser = new Customer(id, pw); // currentUser set
    }
    else {
        std::cout << "ID or password is incorrect." << endl;
    }
}

//origin BookRecommender.cpp

void BookRecommender::countGenre(vector<Book*> history) {
    this->genreCount.insert(make_pair("Literature", 0));
    this->genreCount.insert(make_pair("Non_fiction", 0));
    this->genreCount.insert(make_pair("Practical", 0));
    this->genreCount.insert(make_pair("TeenAndChild", 0));

    for (int i = 0; i < history.size(); ++i) {
        auto it = this->genreCount.find(history[i]->getGenre());
        it->second += 1;
    }
}

void BookRecommender::countAuthors(vector<Book*> history) {

    for (int i = 0; i < history.size(); ++i) {
        auto it = authorCount.find(history[i]->getAuthor());
        if (it == authorCount.end()) // ���ڰ� ���ٸ�
            authorCount.insert(make_pair(history[i]->getAuthor(), 1));
        else
            it->second += 1;
    }
}
void BookRecommender::countLang(vector<Book*> history) {

    for (int i = 0; i < history.size(); ++i) {
        auto it = authorCount.find(history[i]->getLang());
        if (it == authorCount.end()) // ���ڰ� ���ٸ�
            authorCount.insert(make_pair(history[i]->getLang(), 1));
        else
            it->second += 1;
    }
}
void BookRecommender::analyzeHistory(vector<Book*> history) {}
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
        std::cout << "**" << order[i] << " Recommendations\n";
        cnt = 0;
        auto rangeItr = recommendResult.equal_range(order[i]);
        for (auto it = rangeItr.first; it != rangeItr.second; ++it) {
            std::cout << cnt << ". " << it->second->getTitle() << endl;
        }
    }
}
void BookRecommender::readBookHistory(ostream* bookDatabase,
    vector<Book*>& history) {}
