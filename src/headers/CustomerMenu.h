#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <map>

#include "User.h"

// 그냥 책 관련된 것들은 밖으로 뺀 거임. 
// 이름을 어떻게 붙일지 모르겠어서 BookStorage라고 함.
// bookDB, books 등은 customermenu, 검색, 구매, 추천 커맨드 클래스 모두 같은 것을 공유해야 한다고 생각했기 때문임.
class BookStorage {
public:
    BookStorage();
public: 
/////////////// 여기서 public으로 설정하는 거 괜춘?????
    multimap<string, shared_ptr<Book>> books;
    //shared_ptr<Customer> currentUser;
    fstream bookDB;
    array<string, 4> kindOfGenre = {"Literature", "Practical", "Non_fiction", "TeenAndChild"};
};

class CustomerMenu {
public:
    CustomerMenu();

    void addCommand(unique_ptr<Command> com);
    void displayCommands();
private:
    vector<unique_ptr<Command>> commands;
    shared_ptr<BookStorage> bs;
protected:
    
};

class Command{
public:
    Command(shared_ptr<BookStorage> _bs) : bs(_bs) { }

    virtual void execute() = 0;
    void PrintBooks(string chosenGenre);
protected:
    shared_ptr<BookStorage> bs;
};

class OpenLibraryCommand : public Command {
public:
    OpenLibraryCommand(shared_ptr<BookStorage> _bs) : Command(_bs) { }
    virtual void execute() override;
    void PrintBookInfo(string chosenGenre, int BookNumber);
};

class PurchaseBookCommand : public Command {
public:
    PurchaseBookCommand(shared_ptr<BookStorage> _bs) : Command(_bs) { }

    virtual void execute() override;
    void purchase(string chosenGenre, int BookNumber);
};

class GetRecommendationCommand : public Command {
public:
    GetRecommendationCommand(shared_ptr<BookStorage> _bs) : Command(_bs) { }

    virtual void execute() override;
};