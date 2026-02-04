#ifndef STACK_H
#define STACK_H

//typedef - creates an alias for a type; bascially renames a type.
// typedef <extent type> <new type>

typedef struct stack_item {
    int value;
    struct stack_item *next;
} stack_item;

struct stack {
    stack_item *top;
    int size;
};
typedef struct stack stack;

int stack_init(stack *s);

int stack_destroy(stack *s);

int stack_push(stack *s, int val);

int stack_pop(stack *s, int *val);

int stack_peek(stack *s, int *val);

int stack_size(stack *s, int *size);

#endif