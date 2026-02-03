#include "stack.h"

int stack_init(struct stack *s){
    s->top = NULL;
    s->size = 0;

    return 0;

}

int stack_destroy(struct stack *s){
    struct stack_item *tmp;
    while(tmp = s->top) {
        s->top = s->top->next;
        free(tmp);
    }

    // if we make this for people to use, we'd want this so that the size stays consistent, we would need to specify that is IS an error to use a destroyed stack,
    // and reset the values to default.
    s->size = 0;

    return 0;
}

int stack_push(struct stack *s, int val){
    struct stack_item *tmp;

   if(!(tmp = malloc(sizeof(*tmp)))) {
    // if malloc fails
    return -1;
   }
   tmp->value = val;
   tmp->next = s->top;
   s->top = tmp;
   s->size++;

   return 0;
}

int stack_pop(struct stack *s, int *val){
    
    struct stack_item *tmp;

    if(!s->top) {
        return -1;
    }

    tmp = s->top;

    *val = s->top->value;
    s->top = s->top->next;

    s->size--;

    free(tmp);

    return 0;
}

int stack_peek(struct stack *s, int *val){
    
    if(!s->top) {
        return -1;
    }

    *val = s->top->value;

    return 0;
}

int stack_size(struct stack *s, int *size){
    *size = s->size;

    return 0;
}