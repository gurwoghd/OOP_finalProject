#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <array>

#include "headers/CustomerMenu.h"
#include "headers/User.h"
#include "headers/BookStorage.h"

using namespace std;

BookStorage::BookStorage() {
    bookDB.open(filepath, fstream::in);

    try {
        // if not opened
        if(!bookDB.is_open()) throw DatabaseNotOpen();
        // books 벡터 만들기
        string line;
        while(getline(bookDB, line)) {
            string token;
            stringstream ss(line);
            vector<string> a;
            while(getline(ss, token, ' ')) {
                a.push_back(token);
            }

            shared_ptr<Book> book;
            if(a[6] == "Literature") book = make_shared<Literature>(a[0], a[1], a[2], a[3], a[4], stof(a[5]));
            else if(a[6] == "Non_fiction") book = make_shared<Non_fiction>(a[0], a[1], a[2], a[3], a[4], stof(a[5]));
            else if(a[6] == "Practical") book = make_shared<Practical>(a[0], a[1], a[2], a[3], a[4], stof(a[5]));
            else if(a[6] == "TeenAndChild") book = make_shared<TeenAndChild>(a[0], a[1], a[2], a[3], a[4], stof(a[5]));
            books.insert(make_pair(a[6], book));
        }
        bookDB.close();
    }catch(DatabaseNotOpen& e) {
        cout << e.what() << endl;
    }
}
