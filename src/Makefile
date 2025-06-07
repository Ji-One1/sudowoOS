CC=gcc
CFLAGS=-Wall -Wextra -std=c11

mysh: shell.c interpreter.c shellmemory.c utils.c scheduler.c pcb.c
	$(CC) $(CFLAGS) -c shell.c interpreter.c shellmemory.c utils.c scheduler.c pcb.c
	$(CC) $(CFLAGS) -o mysh shell.o interpreter.o shellmemory.o utils.o scheduler.o pcb.o

clean:
	rm mysh; rm *.o