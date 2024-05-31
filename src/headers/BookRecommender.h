#pragma once

#include <iostream>
#include <array>
#include <vector>
#include <string>
#include <memory>
#include "Book.h"

using namespace std;


class BookRecommender {
private:
    array<vector<unique_ptr<Book>>, 4> books;
    array<int, 4> genreCount;
    vector<unique_ptr<Author>> authorCount;
    vector<unique_ptr<Language>> languageCount;
    array<vector<DetailGenre>, 4> detailGenre;
    array<vector<unique_ptr<Book>>, 4> recommendResult;
public:
    void viewResult();
    array<int, 4> countGenre(vector<shared_ptr<Book>> history);
    
};

class Author {
public:
    string name;
    int cnt;
};

class Language {
public:
    string lang;
    int cnt;
};

class DetailGenre {
public:
    string name;
    int cnt;
};