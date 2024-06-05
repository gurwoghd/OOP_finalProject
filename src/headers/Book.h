#pragma once

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Book {
public:
    Book(string date, string title, string author, string publisher, string lang, float price, string genre) 
        : date(date), title(title), author(author), publisher(publisher), lang(lang), price(price), genre(genre) { }
    void setPublisher(const string publisher) { this->publisher = publisher; }
    void setTitle(const string title) {this->title = title;}
    void setPrice(const float price) {this->price=price;}
    void setDate(const string date) {this->date = date;}
    void setAuthor(const string author) {this->author = author;}
    void setLang(const string lang) {this->lang = lang;}    

    string getPublisher() const {return this->publisher;}
    string getTitle() const {return this->title;}
    float getPrice() const {return this->price;}
    string getDate() const {return this->date;}
    string getAuthor() const { return this->author;}
    string getLang() const {return this->lang;}    
    string getGenre() const {return this->genre;}

    void viewInfo() const;

protected:
    string date;
    string title;
    string author;
    string publisher;
    string lang;
    float price;
    string genre;
    vector<string> detailGenre;

    
private:
    friend class BookManager;
    friend class BookRecommender; //오타 수정함//
};

//Genre별 constructor 추가했습니다.//
class Literature : public Book {
public:
    Literature(string date, string title, string author, string publisher, string lang, float price):
Book(date, title, author, publisher, lang, price, "Literature"){}
};

class Non_fiction : public Book {
public:
    Non_fiction(string date, string title, string author, string publisher, string lang, float price):
Book(date, title, author, publisher, lang, price, "Non_fiction"){}
};

class Practical : public Book {
public:
    Practical(string date, string title, string author, string publisher, string lang, float price):
Book(date, title, author, publisher, lang, price, "Practical"){}

};


class TeenAndChild : public Book {
public:
    TeenAndChild(string date, string title, string author, string publisher, string lang, float price):
Book(date, title, author, publisher, lang, price, "TeenAndChild"){}

};
