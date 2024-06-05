#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "User.h"


class OpenLibraryCommand : public Command {
public:
    void execute() override {
        cout << "\nOpen Library selected.\n" << endl;
        // Add further implementation here
    }
};

class PurchaseBookCommand : public Command {
public:
    void execute() override {
        cout << "\nPurchase Book selected.\n" << endl;
        // Add further implementation here
    }
};

class GetRecommendationCommand : public Command {
public:
    void execute() override {
        cout << "\nGet Recommendation selected.\n" << endl;
        // Add further implementation here
    }
};
