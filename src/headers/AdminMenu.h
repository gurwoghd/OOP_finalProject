#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "Book.h"
#include "User.h"
#include "LoginMenu.h"

using namespace std;

class AdminMenu {
public:
    void addCommand(shared_ptr<Command> command);
    void displayCommands();
    void executeCommand();
}

// abstract
class ManageCommands : public Command {
public:
    void execute();
}

class AddBook : public ManageCommands {
public:
    void execute();
}

class DeleteBook : public ManageCommands {
public:
    void execute();
}

void addAction() {

}

void deleteAction() {
    
}