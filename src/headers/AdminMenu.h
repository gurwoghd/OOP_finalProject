#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "Book.h"
#include "User.h"

using namespace std;

class BookStorage {
public:
    BookStorage();
public: 
/////////////// 여기서 public으로 설정하는 거 괜춘?????
    multimap<string, shared_ptr<Book>> books;
    //shared_ptr<Customer> currentUser;
    fstream bookDB;
    array<string, 4> kindOfGenre = {"Literature", "Practical", "Non_fiction", "TeenAndChild"};
    string filepath = "../databases/BookDatabase.txt";
};

class Command {
public:
    Command(shared_ptr<BookStorage> _bs) : bs(_bs) { }

    virtual void execute() = 0;
    virtual ~Command() {}
private:
    shared_ptr<BookStroage> bs;
};

class AdminMenu {
public:
    AdminMenu();

    void addCommand(shared_ptr<Command> command);
    void displayCommands();
private:
    vector<shared_ptr<Command>> commands;
};

// abstract
class ManageCommands : public Command {
public:
    ManageCommands(shared_ptr<BookStorage> _bs) : Command(_bs) { }

    virtual void execute() override;
};

class AddBook : public Command {
public:
    AddBook(shared_ptr<BookStorage> _bs) : Command(_bs) { }
    virtual void execute() override;
};

class DeleteBook : public Command {
public:
    DeleteBook(shared_ptr<BookStorage> _bs) : Command(_bs) { }

    void printBooks(string selectedGenre) const;
    virtual void execute() override;
};
