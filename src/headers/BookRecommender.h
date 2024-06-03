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
    multimap<string, int> countGenre(vector<shared_ptr<Book>> history);
    vector<shared_ptr<Author>> countAuthors(vector<shared_ptr<Book>> history);
    vector<shared_ptr<Language>> countLang(vector<shared_ptr<Book>> history);
    void analyzeHistory(vector<shared_ptr<Book>> history);
    void makeRecommendation(multimap<string, shared_ptr<Book>> & recommendResult);
    void sortRecommendationByCount(multimap<string, shared_ptr<Book>> & recommendResult);
    void sortRecommendationByDate(multimap<string, shared_ptr<Book>> & recommendResult);
    void sortRecommendationByPrice(multimap<string, shared_ptr<Book>> & recommendResult);
    void sortRecommendation(multimap<string, shared_ptr<Book>> & recommendResult);
    void printRecommendation();
    void readBookHistory(shared_ptr<ostream> bookDatabase, vector<shared_ptr<Book>> & history);
private:
    multimap<string, unique_ptr<Book>> books;
    multimap<string, int> genreCount;
    vector<unique_ptr<Author>> authorCount;
    vector<unique_ptr<Language>> languageCount;
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