#include <iostream>
#include <memory>

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

int main(){
    shared_ptr<A> a;
    auto c = make_unique<C>(a);
    return 0;
}