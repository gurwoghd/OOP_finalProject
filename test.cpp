#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

int main(){
    fstream file;
    string filename = "databases/UserDatabase.txt";

    file.open(filename);
    if(file.is_open()) {
        string line;
        stringstream ss(line);
        while(getline())
        vector<string> a = split(line, ' ');
        
        getline(file, line);
        cout << line << endl;
        cout << a[0] << endl << a[1] << endl;
        // getline(file, line);
        // cout << line << endl;
    }else{
        cout << "not opened";
    }
    file.close();
}