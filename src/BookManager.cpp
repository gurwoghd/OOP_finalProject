#include <iostream>
#include <string>
#include <memory>

#include "headers/BookManager.h"

using namespace std;


void BookManager::addBook(Book& book, string genre) {
    books.insert(make_pair(genre, make_unique<Book>(book)));
}

void BookManager::deleteBook(string genre, string title) {
    auto rangeIter = books.equal_range(genre);
    for(auto it = rangeIter.first; it != rangeIter.second; ++it) {
        // it->first는 key, second는 value이다.
        if(it->second->title == title) {
            books.erase(it);
            break;
        }
    }
}