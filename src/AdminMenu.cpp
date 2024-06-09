#include <iostream>
#include <memory>
#include <vector>

#include "headers/AdminMenu.h"
#include "headers/BookManager.h"
#include "headers/User.h"

using namespace std;

AdminMenu::AdminMenu() {
    this->addCommand(make_shared<ManageCommands>());
    this->addCommand(make_shared<AddBook>());
    this->addCommand(make_shared<DeleteBook>());
}

void AdminMenu::addCommand(shared_ptr<Command> command) {
    this->commands.push_back(command);
}

void AdminMenu::displayCommands() {
    
}

