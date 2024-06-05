#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <map>

#include "User.h"

class CustomerMenu {
public:
    CustomerMenu();

    void addCommand(unique_ptr<Command> com);
    void displayCommands();
    void executeCommand();
private:
    vector<unique_ptr<Command>> commands;
    shared_ptr<Customer> currentUser;
protected:
    multimap<string, shared_ptr<Book>> books;
    fstream bookDB;
};

class Command : public CustomerMenu {
public:
    virtual void execute() = 0;
    void PrintBooks(string chosenGenre);
};

class OpenLibraryCommand : public Command {
public:
    virtual void execute() override;
    void PrintBookInfo(string chosenGenre, int BookNumber);
};

class PurchaseBookCommand : public Command {
public:
    virtual void execute() override;
    void purchase(string chosenGenre, int BookNumber);
};

class GetRecommendationCommand : public Command {
public:
    virtual void execute() override;
};

class LogoutCommand : public Command {
public:
    virtual void execute() override;
}