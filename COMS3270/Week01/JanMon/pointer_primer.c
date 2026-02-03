#include <stdio.h>
#include <stdlib.h>

//static means known at compile time
//dynamic means done after compile

/* a pointer simply is a variable that holds an address. */
/* in java, everything is a pointer! (except primitives like int) */

/* no pointers, no work.
void swap(int x, int y)
{
	int tmp;
	
	tmp = x;
	x = y;
	y = tmp;
}
*/



/* C only has the ability to pass by value, so all this does is swap the addresses in the temp variables in the function, but never moves the data.
void swap(int *x, int *y)
{
	int *tmp;
	
	tmp = x;
	x = y;
	y = tmp;
}
*/


void swap(int *x, int *y)
{
	int tmp;
	/* unary* is the derefrence op
	erator (opposite of the * address operator) that looks inside a pointer (by dereferencing the address) 
	and then gives the value stored. */
	
	tmp = *x;
	*x = *y;
	*y = tmp;
}


struct foo {
	int i;
	int j;
	
};
/* This passes a copy of the struct, so it does not initialize the parameter.
// void initialize_foo(struct foo f)
*/
void initialize_foo(struct foo *f) 
{
	// .(dot) gets us to fields in a struct.
	// -> gets us to field in a pointer to a struct
	f->i = 0;
	f->j = 1;
}

// char * is usually a string, but technically it's a pointer to char
// in C a string is simply an array of chars that is terminated by a NULL byte.

int main(int argc, char *argv[])
{
	int i, j;
	struct foo f;
	struct foo *p;
	f.i = 5;
	f.j = 6;
	
	initialize_foo(&f);
	
	p = &f;
	
	printf("p->i: %d, p->j: %d\n", p->i,p->j);
	//sizeof() is not a function!
	//dynamic allocation of struct foo
	//malloc is our memory allocator, the parameter is the number of bytes to allocate, and it returns an address to the allocated storage
	//if malloc fails, it will return a NULL on failure, you NEED to check for this.
	if(!(p = malloc(sizeof(*p)))) 
	{
		// this works because NULL is locally false and non-NULL is true.
		printf("malloc FAIL");
		return -1;
	}
	initialize_foo(p);
	printf("p->i: %d, p->j: %d\n", p->i,p->j);
	
	// when we're done with our dynamically allocated storage, we need to explicitly clean up after ourselves.
	//free is the opposite of malloc, if a program is truly correct, every malloc will have a free.
	free(p);
	
	
	i = 0;
	j = 1;
	
	printf("x: %d, y: %d\n", i, j);
	/* & is the address operator, when used on a variable it gives the address in memory of the variable rather than the value (location vs value)*/
	swap(&i,&j);
	
	printf("x: %d, y: %d\n", i, j);
	
	return 0;
}