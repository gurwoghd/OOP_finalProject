#include <iostream>

class A {
public:
    int a;
    A(int _a) : a(_a)  { }
};

class B {
public:
    A& val;
    B() { }

};

class C { // loginasCustomer
public:
    A* val;
    C(A* _val) { _val = val; }
};

int main() {
    A* currentUser;

    B* loginMenu = new B(); // loginMenu
    C* loC = new C(loginMenu->val); 

    loC->val = new A(20);

    std::cout << loginMenu->val << std::endl;
    std::cout << (loC);
    
}