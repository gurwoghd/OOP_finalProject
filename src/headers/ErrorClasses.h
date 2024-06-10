#pragma once

#include <iostream>
#include <stdexcept>

using namespace std;

//////////////////////  Exception clsses  ///////////////////////////////////
class AlreadyExist : public exception {
public:
    virtual string what() {
        return "User already exsits\n";
    }
};

class DatabaseNotOpen : public exception {
public:
    virtual string what()  {
        return "File not opened\n";
    }
};

///////////////////////////////////////////////////////////////