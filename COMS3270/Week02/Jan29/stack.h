#ifndef STACK_H
#define STACK_H

struct stack_item {
    int value;
    struct stack_item *next;
};

struct stack {
    struct stack_item *top;
    int size;
};

int stack_init(struct stack *s);

int stack_destroy(struct stack *s);

int stack_push(struct stack *s, int val);

int stack_pop(struct stack *s, int *val);

int stack_peek(struct stack *s, int *val);

int stack_size(struct stack *s, int *size);

#endif