#include <iostream>
#include <string>
#include <queue>

using namespace std;

class A {
public:
    A(string a, int b, string c) : a(a), b(b), c(c) { }
private:
    string a;
    int b;
    string c;
};

int main() {
    priority_queue<A*> pq;
    pq.push(new A("a", 10, "h"));
    pq.push(new A("b", 10, "h"));
    pq.push(new A("c", 10, "h"));

    std::cout << (pq.top()) << std::endl;
}