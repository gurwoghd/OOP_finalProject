#include <iostream>
#include <array>
enum Genre {A=0, B, C};
using namespace std;

int main(){
    array<int, 4> a;

    a[Genre::A] = 4;
}