#include <iostream>

using namespace std;

class A {
public:
    A() { }
    virtual void show() = 0;
};

class B : public A {
public:
    B() : A() { }
    virtual void show() override { cout << "hello\n"; }
};

class C {
public:
    C(shared_ptr<A> a) : _a(a) { }
private:
    shared_ptr<A> _a;
};

int main01(){
    cout << "hello\n";
    return 0;
}