#include <iostream>
#include <vector>
#include <memory>
#include <array>

#include "headers/BookRecommender.h"
#include "headers/Book.h"

using namespace std;


void BookRecommender::countGenre(vector<shared_ptr<Book>> history) {
    this->genreCount.insert(make_pair("Literature", 0));
    this->genreCount.insert(make_pair("Non_fiction", 0));
    this->genreCount.insert(make_pair("Practical", 0));
    this->genreCount.insert(make_pair("TeenAndChild", 0));

    for(int i = 0; i < history.size(); ++i) {
        auto it = this->genreCount.find(history[i]->getGenre());
        it->second += 1;
    }
}

void BookRecommender::countAuthors(vector<shared_ptr<Book>> history){

    for(int i = 0; i < history.size(); ++i) {
        auto it = authorCount.find(history[i]->getAuthor());
        if(it == authorCount.end()) // 저자가 없다면
            authorCount.insert(make_pair(history[i]->getAuthor(), 1));
        else
            it->second += 1;
    }
}
void BookRecommender::countLang(vector<shared_ptr<Book>> history){

    for(int i = 0; i < history.size(); ++i) {
        auto it = authorCount.find(history[i]->getLang());
        if(it == authorCount.end()) // 저자가 없다면
            authorCount.insert(make_pair(history[i]->getLang(), 1));
        else
            it->second += 1;
    }
}
void BookRecommender::analyzeHistory(vector<shared_ptr<Book>> history){
    
}
void BookRecommender::makeRecommendation(){
    sortRecommendationByCount();
    sortRecommendationByDate();
    sortRecommendationByPrice();
}
void BookRecommender::sortRecommendationByCount(){
    
}
void BookRecommender::sortRecommendationByDate(){
    
}
void BookRecommender::sortRecommendationByPrice(){
    
}
void BookRecommender::sortRecommendation(){
    
}
void BookRecommender::printRecommendation(){
    array<string, 4> order = {"Literature", "Non_fiction", "Practical", "TeenAndChild"};
    int cnt = 0;
    for(int i = 0; i<4; i++){
        cout << "**" << order[i] << " Recommendations\n";
        cnt = 0;
        auto rangeItr = recommendResult.equal_range(order[i]);
        for(auto it = rangeItr.first; it != rangeItr.second; ++it) {
            cout << cnt << ". " << it->second->getTitle() << endl;
        }
    }
}
void BookRecommender::readBookHistory(shared_ptr<ostream> bookDatabase, vector<shared_ptr<Book>> & history){
    
}