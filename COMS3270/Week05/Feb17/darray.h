#ifndef DARRAY_H
#define DARRAY_H

//opaque data structure is a data structure that the user is unable to modify/see, inner parameters are hidden.

// there exists a struct type darray
struct darray;

// this type exists and here is the name for it, there is a struct darray that points to darray
// this allows for the default definition of a darray to be a pointer even without the * symbol!
typedef struct darray *darray;

int darray_init(darray *d, int element_size);
int darray_delete(darray *d);
// void is the element being added
int darray_add(darray *d, void *v);
// pointer to pointer to pass by address for the void pointer, allows user to see what the element is.
int darray_at(darray *d, int index, void *v);
int darray_num_elements(darray *d, int *capacity);
int darray_remove(darray *d, int index, void **v);
// allows functions to effect the elements using the void visitor.
int darray_visit_all(darray *d, void (*visitor)(void *));



#endif