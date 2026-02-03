#include <stdio.h>
#include <stdlib.h>

int fib(unsigned n) {
	if(n < 2) {
		return n;
	}
	
	return fib(n-1) + fib(n-2);
}


int main(int argc, char * argv[]) {
	printf("F%d = %u\n", atoi(argv[1]), fib(atoi(argv[1])));
	
	return 0;
}