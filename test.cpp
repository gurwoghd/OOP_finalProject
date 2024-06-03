#include <iostream>
#include <map>

using namespace std;

int main(){
    multimap<string, int> books;

    books.insert(make_pair("a", 1));
    books.insert(make_pair("a", 2));
    books.insert(make_pair("b", 1));

    auto rangeIter = books.equal_range("b");
    for(auto it = rangeIter.first; it != rangeIter.second; ++it){
        if(it->second == 1){
            books.erase(it);
            break;
        }
            
    }
    for(auto it = books.begin(); it!=books.end(); ++it){
        cout << it->first << " " << it->second << endl;
    }
}