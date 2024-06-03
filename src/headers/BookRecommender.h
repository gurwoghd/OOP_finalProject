#pragma once

#include <iostream>
#include <array>
#include <vector>
#include <string>
#include <memory>
#include <map>
#include "Book.h"

using namespace std;


class BookRecommender {
public:
    void viewResult();
    void countGenre(vector<shared_ptr<Book>> history);
    void countAuthors(vector<shared_ptr<Book>> history);
    void countLang(vector<shared_ptr<Book>> history);
    void analyzeHistory(vector<shared_ptr<Book>> history);
    void makeRecommendation();
    void sortRecommendationByCount();
    void sortRecommendationByDate();
    void sortRecommendationByPrice();
    void sortRecommendation();
    void printRecommendation();
    void readBookHistory(shared_ptr<ostream> bookDatabase, vector<shared_ptr<Book>> & history);
private:
    multimap<string, unique_ptr<Book>> books;
    multimap<string, int> genreCount;
    multimap<string, int> authorCount;
    multimap<string, int> languageCount;
    multimap<string, unique_ptr<DetailGenre>> detailGenre;
    multimap<string, unique_ptr<Book>> recommendResult;    
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