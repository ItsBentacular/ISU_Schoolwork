#include <cstdio>
#include <iostream>

// Don't do this, the standard namespace is huge (Bjorns book said otherwise, idk.)
//using namespace std;

void cswap(int *x, int *y) {
    int tmp;
    tmp = *x;
    *x = *y;
    *y = tmp;
}

void cppswap(int &x, int &y) {
    int tmp;
    tmp = x;
    x = y;
    y = tmp;
}

int main(int argc, char *argv[]) {
    int i,j;

    i = 0;
    j = 1;

    printf("i= %d, j = %d\n",i, j);
    cswap(&i,&j);
    printf("i= %d, j = %d\n",i, j);

    std::cout << "i= " << i << ", j= " << j << std::endl;
    cppswap(i,j);
    std::cout << "i= " << i << ", j= " << j << std::endl;

    return 0;
}