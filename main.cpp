#include <iostream>
#include "list.h"

void check(list<int> & a) {
    for (auto x : a) {
        std::cerr << x << ' ';
    }
    std::cerr << '\n';
}

int main() {
    list<int> a, b;
    for (size_t i = 1; i <= 4; i++) {
        a.push_back(i);
    }
    swap(a, b);
    return 0;
}