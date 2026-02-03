#include "queue.h"
#include <stdlib.h>

int queue_init(struct queue *q){
    q->front = NULL;
    q->back = NULL;

    return 0;
}

int queue_enqueue(struct queue *q, int x, int y){
    struct queue_item *temp;

    if(!(temp = malloc(sizeof(*temp)))) {
        return -1;
        //malloc failed
    }

    temp->x = x;
    temp->y = y;
    temp->next = NULL;

    if(q->front == NULL && q->back == NULL) {
        q->front = temp;
        q->back = temp;
    } else {
        q->back->next = temp;
        q->back = temp;
    }

}

int queue_dequeue(struct queue *q, int *x, int *y){
    struct queue_item *temp;

    

    free(temp);
}

int queue_getFront(struct queue *q){

}

int queue_getRear(struct queue *q){

}

int queue_getSize(struct queue *q){

}


