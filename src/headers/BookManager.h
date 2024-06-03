#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <array>
#include <map>

#include "Book.h"


using namespace std;

class BookManager {
public:
    BookManager();
    void addBook(Book& book, string genre);
    void deleteBook(string genre, string title);
private:
    multimap<string, unique_ptr<Book>> books;

    friend class Admin;
};