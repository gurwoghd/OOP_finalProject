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

// Different classes, whichever inherited or independent, may use them:
multimap<string, Book*> Customer::accessedBooks;
multimap<string, Book*> Customer::purchasedBooks;
BookRecommender* Customer::bookRecommender;
bool LoginMenu::logined;
User* LoginMenu::currentUser;

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

        // suppose Logout or Exit commands are the last command
        if (selection == commands.size() + 1) break;
        // beyond commands.size() or negative sign commands means not supplied command and 
        else if (selection > commands.size() || selection <= 0)
            std::cout << "Invalid selection, select another one: " << std::endl;
        else
            this->commands[selection - 1]->execute();
    }
}

void Admin::AddBook::execute() {
    int chosenGenre;
    array<string, 4> genres = { "Literature", "Practical", "Non-Fiction",
                               "Teen-and Child" };
    string title, publishDate, author, publisher, language, bookGenre, detailBookGenre;
    float price;
    try {
        bs->bookDB.open(bs->filepath);
        if (bs->bookDB.is_open())
            throw(DatabaseNotOpen("Admin::AddBook::execute()"));

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
        std::cout << "Enter the detail of the genre: ";
        std::cin >> detailBookGenre;

        bs->books.insert(
            make_pair(bs->kindOfGenre[chosenGenre - 1],
                new Book(publishDate, title, author, publisher,
                    language, price, bookGenre, detailBookGenre)));
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

    auto rangeIter = bs->books.equal_range(bs->kindOfGenre[chosenGenre - 1]);
    for (auto it = rangeIter.first; it != rangeIter.second; ++it) {
        if (it->second->getTitle() == title) {
            bs->books.erase(it);
            break;
        }
    }
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
    std::cout << std::endl;
    std::cout << this->getTitle() << endl;
    std::cout << "Date: " << this->getDate() << endl;
    std::cout << "Price: " << this->getPrice() << endl;
    std::cout << "Author: " << this->getAuthor() << endl;
    std::cout << "Publisher: " << this->getPublisher() << endl;
    std::cout << "Language: " << this->getLang() << endl;
    std::cout << "Genre: " << this->getGenre() << endl;
    std::cout << "Detail of the Genre" << this->getDetailGenre() << std::endl;
    std::cout << std::endl;
}

BookManager::BookManager() : books() {}

void BookManager::addBook(Book& book, string genre) {
    books.insert(make_pair(genre, new Book(book)));
}

void BookManager::deleteBook(string genre, string title) {
    auto rangeIter = books.equal_range(genre);
    for (auto it = rangeIter.first; it != rangeIter.second; ++it) {
        if (it->second->title == title) {
            books.erase(it);
            break;
        }
    }
}

BookStorage::BookStorage() {
    bookDB.open(filepath, fstream::in);

    try {
        // if not opened
        if (!bookDB.is_open())
            throw DatabaseNotOpen("BookStorage::BookStorage");
        
        string line;
        while (getline(bookDB, line)) {
            string token;
            stringstream ss(line);
            vector<string> a;
            while (getline(ss, token, ',')) {
                a.push_back(token);
            }

            Book* book;
            if (a[6] == "Literature")
                book = new Literature(a[0], a[1], a[2], a[3], a[4], stof(a[5]), a[7]);
            else if (a[6] == "Non_fiction")
                book = new (Non_fiction)(a[0], a[1], a[2], a[3], a[4], stof(a[5]), a[7]);
            else if (a[6] == "Practical")
                book = new (Practical)(a[0], a[1], a[2], a[3], a[4], stof(a[5]), a[7]);
            else if (a[6] == "TeenAndChild")
                book = new (TeenAndChild)(a[0], a[1], a[2], a[3], a[4], stof(a[5]), a[7]);
            books.insert(make_pair(a[6], book));
        }
        bookDB.close();
    }
    catch (DatabaseNotOpen& e) {
        std::cout << e.what() << endl;
    }
}


Customer::CustomerMenu::CustomerMenu() {
    bs = new BookStorage();
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

        if (selection == commands.size() + 1) break;
        else if (selection > commands.size() || selection <= 0)
            std::cout << "Invalid selection, select another one: ";
        else
            this->commands[selection - 1]->execute();
    }
}

enum genre { LITERATURE = 1, PRACTICAL, NON_FICTION, TEEN_AND_CHILD };

void Customer::OpenLibraryCommand::execute() {
    int chosenGenre;
    int BookNumber;

    std::cout << "\nOpen Library selected.\n" << endl;
    std::cout << "Choose a Genre\n";
    std::cout << "1.Literature 2. Practical 3. Non_fiction 4. Teen-and Child: ";
    std::cin >> chosenGenre;

    if (chosenGenre == genre::LITERATURE){
        if(empty("Literature")) { std::cout << "Empty\n\n"; return;}
        PrintBooks("Literature");
    }
    else if (chosenGenre == genre::PRACTICAL){
        if(empty("Practical")) { std::cout << "Empty\n\n"; return;}
        PrintBooks("Practical");
    } else if (chosenGenre == genre::NON_FICTION) {
        if(empty("Non_fiction")) { std::cout << "Empty\n\n"; return;}
        PrintBooks("Non_fiction");
    }
    else if (chosenGenre == genre::TEEN_AND_CHILD) {
        if(empty("TeenAndChild")) { std::cout << "Empty\n\n"; return;}
        PrintBooks("TeenAndChild");
    }

    while (true) {
        std::cout << "\nSelect a book to open, or enter 0 to exit the library: ";
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

void Customer::OpenLibraryCommand::PrintBookInfo(string chosenGenre, int BookNumber) {
    auto BooksIter = bs->books.equal_range(chosenGenre);
    auto it = BooksIter.first;
    for (int i = 0; i < BookNumber; i++)
        ++it;
    --it;
    auto selectedBook = it->second;
    selectedBook->viewInfo();

    // when view info of books, then the books are uploaded into accessedBooks
    Customer::accessedBooks.insert(make_pair(chosenGenre, selectedBook));
}

void BookRecommender::printRecomentation() {
       std::cout << "**" << "Literature" << " Recommendations\n";
    std::cout << "1. " << "The Great Gatsby" << std::endl;
    std::cout << "2. " << "The Kite Runner" << std::endl;
    std::cout << "3. " << "Harry Potter and the Philosopher's Stone" << std::endl << std::endl;

    std::cout << "**" << "Practical" << std::endl;
    std::cout << "1. " << "The Life-Changing Magic of Tidying Up" << std::endl;
    std::cout << "2. " << "The Life-Changing Magic of Tidying Up" << std::endl;
    std::cout << "3. " << "The Power of Now,Eckhart Tolle" << std::endl << std::endl;

    std::cout << "**" << "Non_Fiction" << std::endl;
    std::cout << "1. " << "Fear: Trump in the White House" << std::endl;
    std::cout << "2. " << "Capital in the Twenty-First Century" << std::endl;
    std::cout << "3. " << "Fast Food Nation" << std::endl << std::endl;

    std::cout << "**" << "Teen-and Child" << std::endl;
    std::cout << "1. " << "The Hunger Games" << std::endl;
    std::cout << "2. " << "The Lion the Witch and the Wardrobe" << std::endl;
    std::cout << "3. " << "Harry Potter and the Philosopher's Stone" << std::endl << std::endl;
}

void Customer::PurchaseBookCommand::execute() {
    int chosenGenre;
    int BookNumber;
    short continuePurchase;

    std::cout << "\nPurchase Book selected.\n" << endl;
    std::cout << "Select the number of a Genre you are intrested" << endl;
    std::cout << "1. Literature 2. Practical 3. Non-fiction 4. Teen-and Child: ";
    std::cin >> chosenGenre;

    if (chosenGenre == genre::LITERATURE){
        if(empty("Literature")) { std::cout << "Empty\n\n"; return;}
        PrintBooks("Literature");
    }
    else if (chosenGenre == genre::PRACTICAL){
        if(empty("Practical")) { std::cout << "Empty\n\n"; return;}
        PrintBooks("Practical");
    } else if (chosenGenre == genre::NON_FICTION) {
        if(empty("Non_fiction")) { std::cout << "Empty\n\n"; return;}
        PrintBooks("Non_fiction");
    }
    else if (chosenGenre == genre::TEEN_AND_CHILD) {
        if(empty("TeenAndChild")) { std::cout << "Empty\n\n"; return;}
        PrintBooks("TeenAndChild");
    }
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

// utility function for purchase menu command
void Customer::PurchaseBookCommand::purchase(string chosenGenre, int BookNumber) {
    auto selectedBookIter = bs->books.equal_range(chosenGenre);
    auto it = selectedBookIter.first;
    for (int i = 0; i < BookNumber; i++)
        ++it;
    --it;
    auto selectedBook = it->second;

    // After purchasing books, they're uploaded on the purchasedBooks.
    Customer::purchasedBooks.insert(
        make_pair(chosenGenre, selectedBook));

    std::cout << selectedBook->getTitle() << " has been successfully purchased"
        << endl;
}

void Customer::GetRecommendationCommand::execute() {
    std::cout << "\nGet Recommendation selected.\n" << endl;
    Customer::getBookRecommender()->printRecommendation();
}

LoginMenu::LoginMenu() {
    this->addCommands(new RegisterCommand());
    this->addCommands(new LoginasAdmin());
    this->addCommands(new LoginasCustomer());

    LoginMenu::logined = false;
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

        if (selection == commands.size() + 1) break; // exit
        else if (selection > commands.size())
            std::cout << "Invalid selection, select another one: " << std::endl;
        else if(selection == 1) commands[selection-1]->execute(); // Register
        else { // others
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
    try {
        manager->addNewUser(id, pw);
        std::cout << "\nRegister Complete! You are ready to log in!" << endl;
    }
    catch (DatabaseNotOpen& e) {
        cout << e.what() << endl;
    }
    catch (AlreadyExist& e) {
        cout << e.what() << endl;
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
        LoginMenu::currentUser = new Admin(adminID, adminPW); // currentUser set
        LoginMenu::logined = true;
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
    try{
        if (manager->check(id, pw)) {
            std::cout << "Login successful, welcome " << id << endl;
            LoginMenu::currentUser = new Customer(id, pw); // currentUser set
            LoginMenu::logined = true;
        }
        else {
            std::cout << "ID or password is incorrect." << endl;
        }
    }catch(DatabaseNotOpen& e) {
        std::cout << e.what() << std::endl;
    }
}


void UserManager::addNewUser(const string& id, const string& pw) throw(AlreadyExist, DatabaseNotOpen) {
    // users.push_back(User(id, pw));
    std::cout << filepath << std::endl;
    unique_ptr<User> user = make_unique<Customer>(id, pw);
    ofstream outfile;

    outfile.open(filepath);
    // userDB.open(filepath);
    if (!outfile.is_open()) {
        throw DatabaseNotOpen("UserManager::addNewUser");
    }
    outfile << id << "," << pw << endl;
    outfile.close();
}

bool UserManager::alreadyExistID(const string& id) const {
    string line;
    ifstream readUserDB;
    readUserDB.open(filepath);
    while (getline(readUserDB, line)) {
        string token;
        stringstream ss(line);
        vector<string> a;
        while (getline(ss, token, ',')) {
            a.push_back(token);
        }
        // input ID equals ID in UserDatabase
        if (a[0] == id) return true;
    }
    return false;
}
bool UserManager::check(const string& id, const string& pw) const throw(DatabaseNotOpen) {
    string line;
    ifstream readUserDB;
    readUserDB.open(filepath);
    if(!readUserDB.is_open()) throw DatabaseNotOpen("UserManager::check");
    while (getline(readUserDB, line)) {
        string token;
        stringstream ss(line);
        vector<string> a;
        while (getline(ss, token, ',')) a.push_back(token);
        // for(auto it = a.begin(); it!= a.end(); it++) 
        //     std::cout << *it << std::endl;
        if (a[0] == id && a[1] == pw) return true;
    }
    return false;
}

// 구매한 책 먼저 10권 넣고 조회한 책 10권 넣음
void BookRecommender::makeHistoryList() {

    add(Customer::purchasedBooks,"Literature");
    add(Customer::purchasedBooks,"Practical");
    add(Customer::purchasedBooks,"Non_fiction");
    add(Customer::purchasedBooks,"TeenAndChild");

    add(Customer::accessedBooks,"Literature");
    add(Customer::accessedBooks,"Practical");
    add(Customer::accessedBooks,"Non_fiction");
    add(Customer::accessedBooks,"TeenAndChild");
}

// 삽입 utility function
void BookRecommender::add(multimap<string, Book*> source, string g) {
    auto BookIter = source.equal_range(g);
    int cnt = 0;
    for(auto it = BookIter.first; it != BookIter.second && cnt < 10; ++it) {
        cnt++;
        // std::cout << it->second << std:: endl;
        history.insert(make_pair(g, it->second));
    }
}

void BookRecommender::countDetailGenre() {
    DGCount.insert(make_pair("Fiction", 0));
    DGCount.insert(make_pair("Classic", 0));
    DGCount.insert(make_pair("Historical Fiction", 0));
    DGCount.insert(make_pair("Mystery", 0));
    DGCount.insert(make_pair("Memoir", 0));
    DGCount.insert(make_pair("Biography", 0));
    DGCount.insert(make_pair("History", 0));
    DGCount.insert(make_pair("Science", 0));
    DGCount.insert(make_pair("Fantasy", 0));
    DGCount.insert(make_pair("Adventure", 0));
    DGCount.insert(make_pair("Coming-of Age", 0));
    DGCount.insert(make_pair("Theatre", 0));

    
    count("Literature");
    count( "Practical");
    count("Non_fiction");
    count("TeenAndChild");
}

void BookRecommender::count(string g) {
    auto BookIter = history.equal_range(g);


    for(auto it = BookIter.first; it != BookIter.second; ++it) {
        (DGCount.find(it->second->getDetailGenre())->second)++;
    }
}

void BookRecommender::findMostCount() {
    string a[15] = {
        "Fiction",
        "Classic",
        "Historical Fiction",
        "Mystery",
        "Memoir",
        "Biography",
        "History",
        "Science",
        "Fantasy",
        "Adventure",
        "Coming-of Age",
        "Theatre",
    };
    int M = -1;
    string _k;
    for(int i=0; i<4; i++) {
        for(int j=4*i; j<4*(i*1); j++) {
            if(M <= DGCount.find(a[j])->second) {
                M = DGCount.find(a[j])->second;
                _k = DGCount.find(a[j])->first;
            }
        }
        mostCount.push_back(_k);
    }
}

void BookRecommender::makeRecommendation() {
    // Literature, Practical, non_fiction, teenandchild 순서
    // mostCount[0]
    ifstream inputfile;
    string filepath = "databases/BookDatabase.txt";
    inputfile.open(filepath);
    if(inputfile.is_open()) {
        string line;
        vector<string> a;
        while(getline(inputfile, line)) {
            string token;
            stringstream ss(line);
            while(getline(ss, token, ',')) {
                a.push_back(token);
            }
            // a[6] : genre, a[7] : detailGenre
            if(a[7] == mostCount[0]) {
                recommendResult[0].push(new Book(a[0], a[1], a[2], a[3], a[4], stof(a[5]), a[6], a[7]));
            } else if(a[7] == mostCount[1])
                recommendResult[1].push(new Book(a[0], a[1], a[2], a[3], a[4], stof(a[5]), a[6], a[7]));
            else if(a[7] == mostCount[2])
                recommendResult[2].push(new Book(a[0], a[1], a[2], a[3], a[4], stof(a[5]), a[6], a[7]));
            else if(a[7] == mostCount[3])
                recommendResult[3].push(new Book(a[0], a[1], a[2], a[3], a[4], stof(a[5]), a[6], a[7]));
        }
    }
}

void BookRecommender::printRecommendation() {
    array<string, 4> order = { "Literature", "Non_fiction", "Practical",
                              "TeenAndChild" };

    makeHistoryList();
    countDetailGenre();
    findMostCount();
    makeRecommendation();
    int cnt = 0;
    for (int i = 0; i < 4; i++) {
        std::cout << "\n**" << order[i] << " Recommendations\n";
        // std::cout << recommendResult[i].size() << std::endl;
        cnt = 1;
        while(!recommendResult[i].empty()){
            std::cout << cnt << ". " << recommendResult[i].top() << std::endl;
            cnt++;
        }
        std::cout << std::endl;
    }
}



























