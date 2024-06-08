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
protected:
    multimap<string, shared_ptr<Book>> books;
    //shared_ptr<Customer> currentUser;
    fstream bookDB;
    array<string, 4> kindOfGenre = {"Literature", "Practical", "Non_fiction", "TeenAndChild"};
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
};