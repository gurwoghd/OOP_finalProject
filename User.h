#pragma once
#include <iostream>
#include<string>
#include<vector>

class User {
private:
	string id;
	string password;
public:
	User(const string& I, const string& P);
	void setID(string I);
	void setPW(string P);

	string getID() const;
	string getPW() const;

};