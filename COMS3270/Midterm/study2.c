#include <stdint.h>

typedef struct list_item
{
    struct list_item *pred, *next;
    void *datum;
} list_item_t;
typedef list_item_t *list_iterator_t;
typedef struct list
{
    list_item_t *head, *tail;
    uint32_t length;
    int32_t (*compare)(const void *key, const void *with);
    void (*datum_delete)(void *);
} 

list_t;
list_t l;
list_item_t *li;
list_iterator_t it;