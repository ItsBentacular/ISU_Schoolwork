#target: list of dependencies
#	command to build target from dependencies (needs a tab at the start)

all: hello times_table fib

hello: hello.c
	gcc hello.c -o hello -g -Wall -Werror
	
times_table: times_table.c
	gcc times_table.c -o times_table -g -Wall -Werror
	
fib: fib.c
	gcc fib.c -o fib -g -Wall -Werror
	
# for assignment 0, assuming program is named assignment0.c, the makefile could be exactly:
# assignment0: asssignment0.c
#	gcc assignment0.c -o assignment0 -g -Wall -Werror
