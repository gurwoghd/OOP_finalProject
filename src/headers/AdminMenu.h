#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "Book.h"
#include "User.h"
#include "ErrorClasses.h"
#include "BookStorage.h"

using namespace std;


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
