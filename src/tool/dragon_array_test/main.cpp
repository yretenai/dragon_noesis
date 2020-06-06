//
// Created by yretenai on 6/5/2020.
//

#include "../../dragon.h"

using namespace dragon;

Array<int> test1() {
    return Array<int>(3);
}

Array<int> test2() {
    short tmp[] = { 1, 0, 2, 0, 3, 0 };
    return Array<int>::cast<short>(tmp, 6);
}

Array<int> test3() {
    int tmp[] = { 1, 2, 3 };
    return Array<int>(tmp, 3);
}

Array<short> test4() {
    short tmp[] = { 1, 0, 2, 0, 3, 0 };
    return Array<short>(tmp, 6);
}

template<typename T>
void print(Array<T>& test) {
    printf("test\n");
    for(int v : test) {
        printf("%d\n", v);
    }
}

int main(int argc, char** argv) {
    Array<int> a = test1();
    print(a);
    a = test2();
    print(a);
    a = test3();
    print(a);
    Array<short> b = test4();
    b[0] = 10;
    b[2] = 20;
    b[4] = 30;
    print(b);
    a = b.cast<int>(0, 3);
    print(a);
    return 0;
}
