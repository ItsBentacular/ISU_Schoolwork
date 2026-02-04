#ifndef QUEUE_H
#define QUEUE_H

typedef enum terrain {
    TERRAIN_ROCK = '%',
    TERRAIN_WATER = '~',
    TERRAIN_GRASS = ':',
    TERRAIN_CLEAR = '.',
    TERRAIN_TREES = '^',
    TERRAIN_ROAD = '#',
    TERRAIN_EMPTY = ' '
} terrain;

typedef struct queue_item {
    int x;
    int y;
    enum terrain t_type;
    struct queue_item *next;

} queue_item;

typedef struct queue {
    struct queue_item *front;
    struct queue_item *back;
} queue;

int queue_init(queue *q);

int queue_enqueue(queue *q, int x, int y, terrain t_type);

int queue_dequeue(queue *q, int *x, int *y, terrain *t_type);

int queue_getFront(queue *q, int *x, int *y, terrain *t_type);

int queue_getRear(queue *q, int *x, int *y, terrain *t_type);

int queue_getSize(queue *q);


#endif