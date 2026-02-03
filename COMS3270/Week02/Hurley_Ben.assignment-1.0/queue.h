#ifndef QUEUE_H
#define QUEUE_H

struct queue_item {
    int x;
    int y;
    struct queue_item *next;

};

struct queue {
    struct queue_item *front;
    struct queue_item *back;
};

int queue_init(struct queue *q);

int queue_enqueue(struct queue *q, int x, int y);

int queue_dequeue(struct queue *q, int *x, int *y);

int queue_getFront(struct queue *q);

int queue_getRear(struct queue *q);

int queue_getSize(struct queue *q);


#endif