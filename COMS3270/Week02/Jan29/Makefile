#this allows builds to be made with prerequisites, going down the list and checking to make sure all files are made, up to date, and available.

stack: stack -o stack_main.o
	gcc stack.o stack_main.o -o stack

stack.o: stack.c stack.h
	gcc -Wall -Werror -g stack.c -c

stack_main.o: stack_main.c stack.h
	gcc -Wall -Werror -g stack_main.c -c

clean:
	rm -f *.o stack *~