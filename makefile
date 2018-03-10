OBJS = bst.o main.o integer.o queue.o sll.o stack.o dll.o real.o scanner.o string.o gst.o avl.o
OOPTS = -Wall -Wextra -g -c
LOPTS = -Wall -Wextra -g
EXEC = ./main -r hs-0-0.data

all : main

main : $(OBJS)
		gcc $(LOPTS) $(OBJS) -o main

string.o : string.c string.h
		gcc $(OOPTS) string.c

scanner.o : scanner.c scanner.h
		gcc $(OOPTS) scanner.c

real.o : real.c real.h
		gcc $(OOPTS) real.c

integer.o : integer.c integer.h
		gcc $(OOPTS) integer.c

sll.o : sll.c sll.h
		gcc $(OOPTS) sll.c

queue.o : queue.c queue.h sll.o
		gcc $(OOPTS) queue.c

bst.o : bst.c bst.h
		gcc $(OOPTS) bst.c

main.o :    main.c integer.o real.o string.o scanner.o
		gcc $(OOPTS) main.c

gst.o : gst.c gst.h bst.o
		gcc $(OOPTS) gst.c

stack.o : stack.c stack.h dll.o
		gcc $(OOPTS) stack.c

dll.o : dll.c dll.h
		gcc $(OOPTS) dll.c

test : main
		$(EXEC)

valgrind : main
		valgrind $(EXEC)

clean	:
		rm -f -r $(OBJS) main *.dSYM

memcheck : main
		valgrind --leak-check=yes $(EXEC)
