#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <array>
#include <map>

#include "headers/AdminMenu.h"
#include "headers/BookManager.h"
#include "headers/User.h"
#include "headers/ErrorClasses.h"

using namespace std;

BookStorage::BookStorage() {
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
        bookDB.close();
    }catch(DatabaseNotOpen& e) {
        cout << e.what() << endl;
    }
}

AdminMenu::AdminMenu() {
    this->addCommand(make_shared<ManageCommands>());
    this->addCommand(make_shared<AddBook>());
    this->addCommand(make_shared<DeleteBook>());
}

void AdminMenu::addCommand(shared_ptr<Command> command) {
    this->commands.push_back(command);
}

void AdminMenu::displayCommands() {
    int selection;
    int chosenGenre;
    array<string, 4> genres = {"Literature", "Practical", "Non-Fiction", "Teen-and Child"};
    string title, publishDate, author, publisher, language, bookGenre;
    float price;
    cout << "Welcome Admin" << endl << endl;

    while(true) {
        cout << "What would you like to do?" << endl << endl;
        cout << "1. Add a book 2. Delete a book 3. Logout: ";
        cin >> selection;

        if(selection >= commands.size() || selection <= 0) cout << "Invalid selection, select another one: ";
        else if(selection == commands.size() - 1) break;
        else this->commands[selection - 1]->execute();
    }
}

void AddBook::execute() {
    try {
        bs->bookDB.open(filepath);
        if(bs->bookDB.is_open()) throw(DatabaseNotOpen);

        cout << "Select the Genre of the book to add" << endl << endl;
        cout << "1. Literature 2. Practical 3. Non-Fiction 4. Teen-and Child: ";
        cin >> chosenGenre;

        cout << "Add a " << genres[chosenGenre - 1] << " Book" << endl << endl;

        cout << "Enter the book name: "; cin >> title;
        cout << "Enter the published date: "; cin >> publishDate;
        cout << "Enter the price: "; cin >> price;
        cout << "Enter the author: "; cin >> author;
        cout << "Enter the publisher: "; cin >> publisher;
        cout << "Enter the language: "; cin >> language;
        cout << "Enter the genre: "; cin >> bookGenre;

        bs->books.insert(make_pair(bs->kindOfGenre[chosenGenre - 1], make_shared<Book>(publishDate, title, author, publisher, language, price, bookGenre)));
        // bookDB에 업데이트
        bookDB << publishDate << " "
                << title << " "
                << author << " "
                << publisher << " "
                << langauage << " "
                << to_string(price) << " "
                << bookGenre << endl;
        bookDB.close();
    } catch(DatabaseNotOpen& e){
        cout << e.what() << endl;
    }
}

void DeleteBook::execute() {
    string title; // title of book to delete
    array<string, 4> genres = {"Literature", "Pracitcal" , "Non-Fiction" , "Teen-and Child"};
    int chosenGenre;

    cout << "Select the Genre of the book to delete" << endl << endl;
    cout << "1. Literature 2. Practical 3. Non-Fiction 4. Teen-and Child : ";
    cin >> chosenGenre;

    this->printBooks(bs->kindOfGenre[chosenGenre - 1]);
    
    cout << "Enter the book name to delete: ";
    cin >> title;

    // 맵에서 제거
    auto rangeIter = bs->books.equal_range(bs->kindOfGenre[chosenGenre - 1]);
    for(auto it = rangeIter.first; it != rangeIter.second; ++it) {
        if(it->second->getTitle() == title){
            bs->books.erase(it);
            break;
        }
    }
    // 데이터베이스에서 제거
    // 데이터베이스에서 title과 일치하는 제목을 포함하는 라인을 찾아서 그 라인만 선택적으로 지워야 함
    
    

}

void DeleteBook::printBooks(string selectedGenre) const {
    auto rangeIter = bs->books.equal_range(selectedGenre);
    int idx = 1;
    for(auto it = rangeIter.first; it != rangeIter.second; ++it){
        cout << idx << ". " << it->second->getTitle() << endl;
    }
}

// 근데 ManageCommands는 무슨 기능???
void ManageCommands::execute() {

}