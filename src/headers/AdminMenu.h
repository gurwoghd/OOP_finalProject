#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "Book.h"
#include "User.h"

using namespace std;

class Command {
public:
    virtual void execute() = 0;
    virtual ~Command() {}
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
    virtual void execute() override;
};

class AddBook : public ManageCommands {
public:
    virtual void execute() override;
};

class DeleteBook : public ManageCommands {
public:
    virtual void execute() override;
};