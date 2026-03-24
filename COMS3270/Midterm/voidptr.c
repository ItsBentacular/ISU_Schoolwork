#include <stdio.h>

int daValue = 21;

void changePointer(void ** something) {
    *something = &daValue;
}

int main(int argc, char * argv[]) {
    /*int integer = 10;
    void * voidPtr;
    voidPtr = &integer;

    *(int *)voidPtr = 5;

    printf("void pointer location is: %p\n", voidPtr);
    printf("integer location is: %p\n", voidPtr);

    printf("void pointer value is: %d\n", *(int *)voidPtr);
    printf("integer value is: %d\n", integer);*/

    int integer = 5;
    int *myPointer = &integer;

    printf("Address is: %p , Value is: %d\n", myPointer, *myPointer);
    printf("Int Address is: %p , Int Value is: %d\n", integer, integer);
    changePointer( (void **)&myPointer);
    printf("Address is: %p , Value is: %d\n", myPointer, *myPointer);
    printf("Int Address is: %p , Int Value is: %d\n", integer, integer);
}