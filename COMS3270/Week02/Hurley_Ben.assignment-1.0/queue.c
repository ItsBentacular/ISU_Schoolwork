#include "queue.h"
#include <stdlib.h>

int queue_init(queue *q){
    q->front = NULL;
    q->back = NULL;

    return 0;
}

int queue_enqueue(queue *q, int x, int y, terrain t_type){
    queue_item *temp;

    if(!(temp = malloc(sizeof(*temp)))) {
        return -1;
        //malloc failed
    }

    temp->x = x;
    temp->y = y;
    temp->t_type = t_type;
    temp->next = NULL;

    if(q->front == NULL && q->back == NULL) {
        q->front = temp;
        q->back = temp;
    } else {
        q->back->next = temp;
        q->back = temp;
    }

    return 0;

}

int queue_dequeue(queue *q, int *x, int *y,terrain *t_type){
    queue_item *temp;

    if(q->front == NULL) {
        return -1;
    }

    temp = q->front;
    *x = q->front->x;
    *y = q->front->y;
    *t_type = q->front->t_type;



    q->front = q->front->next;
        // edge case in case of empty queue
    if(q->front == NULL) {
        q->back = NULL;
    }

    free(temp);

    return 0;
}
// never used this
int queue_getFront(queue *q, int *x, int *y,terrain *t_type){
    if(q->front == NULL) {
        return -1;
        // should not be empty.
    }
    *x = q->front->x;
    *y = q->front->y;
    *t_type = q->front->t_type;

    return 0;
}
// never used this either
int queue_getRear(queue *q, int *x, int *y, terrain *t_type){
    if(q->back == NULL) {
        return -1;
        //should not be empty
    }
    *x = q->back->x;
    *y = q->back->y;
    *t_type = q->back->t_type;

    return 0;
}

int queue_getSize(queue *q){
    queue_item *temp = q->front;
    int size = 0;
    while(temp != NULL) {
        size++;
        temp = temp->next;
    }
    return size;
}


