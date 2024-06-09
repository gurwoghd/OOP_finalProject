#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <map>

#include "User.h"
#include "ErrorClasses.h"
#include "BookStorage.h"

class CustomerMenu {
public:
    CustomerMenu();

    void addCommand(unique_ptr<CustomerMenuCommand> com);
    void displayCommands();
private:
    vector<unique_ptr<CustomerMenuCommand>> commands;
    shared_ptr<BookStorage> bs;
protected:
    
};

class CustomerMenuCommand{
public:
    CustomerMenuCommand(shared_ptr<BookStorage> _bs) : bs(_bs) { }

    virtual void execute() = 0;
    void PrintBooks(string chosenGenre);
protected:
    shared_ptr<BookStorage> bs;
};

class OpenLibraryCommand : public CustomerMenuCommand {
public:
    OpenLibraryCommand(shared_ptr<BookStorage> _bs) : CustomerMenuCommand(_bs) { }
    virtual void execute() override;
    void PrintBookInfo(string chosenGenre, int BookNumber);
};

class PurchaseBookCommand : public CustomerMenuCommand {
public:
    PurchaseBookCommand(shared_ptr<BookStorage> _bs) : CustomerMenuCommand(_bs) { }

    virtual void execute() override;
    void purchase(string chosenGenre, int BookNumber);
};

class GetRecommendationCommand : public CustomerMenuCommand {
public:
    GetRecommendationCommand(shared_ptr<BookStorage> _bs) : CustomerMenuCommand(_bs) { }

    virtual void execute() override;
};