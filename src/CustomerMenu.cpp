#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <array>

#include "headers/CustomerMenu.h"
#include "headers/User.h"

using namespace std;

BookStorage::BookStorage() {
    string filepath = "../databases/BookDatabase.txt";
    bookDB.open(filepath, fstream::in);

    try {
        // if not opened
        if(!bookDB.is_open()) throw DatabaseNotOpen();
        // books 벡터 만들기
        string line;
        while(getline(bookDB, line)) {
            string token;
            stringstream ss(line);
            vector<string> a;
            while(getline(ss, token, ' ')) {
                a.push_back(token);
            }

            shared_ptr<Book> book;
            if(a[6] == "Literature") book = make_shared<Literature>(a[0], a[1], a[2], a[3], a[4], stof(a[5]));
            else if(a[6] == "Non_fiction") book = make_shared<Non_fiction>(a[0], a[1], a[2], a[3], a[4], stof(a[5]));
            else if(a[6] == "Practical") book = make_shared<Practical>(a[0], a[1], a[2], a[3], a[4], stof(a[5]));
            else if(a[6] == "TeenAndChild") book = make_shared<TeenAndChild>(a[0], a[1], a[2], a[3], a[4], stof(a[5]));
            books.insert(make_pair(a[6], book));
        }
    }catch(DatabaseNotOpen& e) {
        cout << e.what() << endl;
    }
}

CustomerMenu::CustomerMenu() {
    bs = make_shared<BookStorage>();
    // command를 추가함
    this->addCommand(make_unique<OpenLibraryCommand>(bs));
    this->addCommand(make_unique<PurchaseBookCommand>(bs));
    this->addCommand(make_unique<GetRecommendationCommand>(bs));
}

void CustomerMenu::addCommand(unique_ptr<Command> com) {
    commands.push_back(move(com));
}

void CustomerMenu::displayCommands() {

    while (true) {
        int selection;
        cout << "1. Open Library" << endl;
        cout << "2. Purchase Book" << endl;
        cout << "3. Get Recommendation" << endl;
        cout << "4. Logout" << endl;
        cout << "\nSelect an option: ";
        cin >> selection;

        if(selection >= commands.size()) cout << "Invalid selection, select another one: ";
        else if(selection == commands.size() - 1) break;
        else this->commands[selection-1]->execute();
    }
}

enum genre {LITERATURE = 1, PRACTICAL, NON_FICTION, TEEN_AND_CHILD};

void OpenLibraryCommand::execute() {
    int chosenGenre;
    short stay = 1;
    int BookNumber;

    cout << "\nOpen Library selected.\n" << endl;
    // Add further implementation here
    cout << "Choose a Genre\n";
    cout << "1.Literature 2. Practical 3. Non_fiction 4. Teen-and Child: ";
    cin >> chosenGenre;

    if(chosenGenre == genre::LITERATURE) PrintBooks("Literature");
    else if(chosenGenre == genre::PRACTICAL) PrintBooks("Practical");
    else if(chosenGenre == genre::NON_FICTION) PrintBooks("Non_fiction");
    else if(chosenGenre == genre::TEEN_AND_CHILD) PrintBooks("TeenAndChild");

    while(true) {
        cout << "Select a book to open, or enter 0 to exit the library: ";
        cin >> BookNumber;

        if(BookNumber == 0) break;
        else {
            this->PrintBookInfo(bs->kindOfGenre[chosenGenre - 1], BookNumber);
        }
    }
}

void Command::PrintBooks(string chosenGenre) {
    auto selectedGenreBooksIter = bs->books.equal_range(chosenGenre);
    int cnt = 1;
    cout << chosenGenre << endl;
    for(auto it = selectedGenreBooksIter.first; it != selectedGenreBooksIter.second; ++it){
        cout << cnt << ". " << it->second->getTitle() << endl;
        cnt++;
    }
}

void OpenLibraryCommand::PrintBookInfo(string chosenGenre, int BookNumber) {
    auto BooksIter = bs->books.equal_range(chosenGenre);
    auto it = BooksIter.first;
    for(int i = 0; i < BookNumber;)
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

    if(chosenGenre == LITERATURE) this->PrintBooks("Literature");
    else if(chosenGenre == PRACTICAL) this->PrintBooks("Practical");
    else if(chosenGenre == NON_FICTION) this->PrintBooks("Non_fiction");
    else if(chosenGenre == TEEN_AND_CHILD) this->PrintBooks("TeenAndChild");

    while(true){
        cout << "Enter the number of the book you want to purchase: ";
        cin >> BookNumber;

        this->purchase(bs->kindOfGenre[chosenGenre - 1], BookNumber);
        cout << "1. Exit 2. Continue purchasing: ";
        cin >> continuePurchase;

        if(continuePurchase == 1) {
            break;
        } else if(continuePurchase == 2) {
            continue;
        }
    }

}

void PurchaseBookCommand::purchase(string chosenGenre, int BookNumber) {
    auto selectedBookIter = bs->books.equal_range(chosenGenre);
    auto it = selectedBookIter.first;
    for(int i = 0; i < BookNumber;)
        ++it;
    --it;
    auto selectedBook = it->second;
    Customer::getPurchaseBookHistory().insert(make_pair(chosenGenre, selectedBook));

    cout << selectedBook->getTitle() << " has been successfully purchased" << endl;
}


void GetRecommendationCommand::execute() {
    cout << "\nGet Recommendation selected.\n" << endl;
    // Add further implementation here
    Customer::getBookRecommender()->printRecommendation();
}
