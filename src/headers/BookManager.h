#pragma once

#include <iostream>
#include <string>
#include <memory>
#include "Book.h"
#include <array>

using namespace std;

class BookManager {
public:
    void addBook();
    void deleteBook();
private:
    array<vector<unique_ptr<Book>>, 4> books;

    friend class Admin;
};