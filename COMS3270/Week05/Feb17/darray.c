#include "darray.h"
#include <stdlib.h>


#define DARRAY_DEFAULT_CAPACITY 10

struct darray {
    // char for single byte indexing vs voids unknown 8 bytes
    char *a;
    int elements;
    int capacity;
    int element_size;
};

int darray_init(darray *d, int element_size){
    if(!(*d = malloc(sizeof(struct darray)))) {
        return -1;
    }

    // always capacity times element size
    if(!((*d)->a = malloc(DARRAY_DEFAULT_CAPACITY * element_size))){
        free(*d);
        return -1;
    }

    (*d)->elements = 0;
    (*d)->capacity = DARRAY_DEFAULT_CAPACITY;
    (*d)->element_size = element_size;

    // calloc opens memory with 0s, malloc opens memory with undefined, THEREFORE, malloc is faster, and calloc should be used as needed.
}

int darray_delete(darray *d){
    // c does not have a garbage collector, so you need to free the pointers 'hanging' from the main struct first, or else they will stay allocated while being useless.
    // arrow has higher precedence than the star, so the parentheses is neededs for correct ordering mathmatically.

    free((*d)->a);
    free((*d));

    return 0;
}

// by both defining it as static and not including it in the header, we essentially have made this a private function.
//static function means that this function can only be called in this file
static int grow(darray *d){
    // this uses a temporary value so that if an error were to occur, the data would not be lost in reallocation.
    void *v;
    if(!(v = realloc((*d)->a, (*d)->capacity * 2 * (*d)->element_size))) {
        return -1;
    }
    // if reallocation succeeds, put new capacity into darray.
    (*d)->a = v;
    (*d)->capacity *= 2;

    return 0;
} 

int darray_add(darray *d, void *v){
    // when capacity == elements, array is full, needs to be reallocated.
    if(!(((*d)->capacity == (*d)->elements) && grow(d))) {
        return -1;
    }

    memcpy((*d)->a + ((*d)->element_size * (*d)->elements), v, (*d)->element_size);

    (*d)->elements++;

    return 0;
}

// for malloc, p = malloc
// p points to the data, and some negative offset points to malloc's internal metadata.

int darray_at(darray *d, int index, void *v){

    if(index >= (*d)->elements || index < 0) {
        return -1;
    }
    // the elements + index * element size gives the offset to the address of the element, basically gives the location of the element. works all the time.
    memcpy(v, (*d)->elements + (index * (*d)->element_size), (*d)->element_size );

    return 0;

}

int darray_num_elements(darray *d, int *capacity){

}

int darray_remove(darray *d, int index, void **v){

}

int darray_visit_all(darray *d, void (*visitor)(void *)){

}