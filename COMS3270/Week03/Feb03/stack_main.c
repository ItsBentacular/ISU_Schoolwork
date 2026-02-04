#include "stack.h"
#include <stdio.h>

int main(int argc, char *argv[]) {

    stack s;
    int i;
    int size;
    if(!(stack_init(&s))) {
        return -1;
        // failed initialization
    }

    stack_size(&s, &size);
    printf("%d\n", size);

    for(i = 0; i < 100; i++) {
        // pushes values 0 to 99 onto the stack
        // good code can fail, and good code should ALWAYS check for failure.
        // this does not, but it should!
        stack_push(&s, i);
    }

    stack_size(&s, &size);
    printf("%d\n", size);

    while(!stack_pop(&s, &i)) {
        printf("value: %d", i);
    }


    return 0;

}