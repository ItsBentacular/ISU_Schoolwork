#include <iostream>
using namespace std;


void swap(int *x, int *y) {
    int tmp;

    tmp = *x;
    *x = *y;
    *y = tmp;

}

int main(int argc, char *argv[]) {

    int i, j;

    i = 0;
    j = 1;

    cout << "i= " << i << "j= " << j;
    return 0;
}