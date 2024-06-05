#include <iostream>

#include "Book.h"

using namespace std;

void Book::viewInfo() const {
    cout << this->getTitle() << endl;
    cout << "Date: " << this->getDate() << endl;
    cout << "Price: " << this->getPrice() << endl;
    cout << "Author: " << this->getAuthor() << endl;
    cout << "Publisher: " << this->getPublisher() << endl;
    cout << "Language: " << this->getLang() << endl;
    cout << "Genre: " << this->getGenre() << endl;
}