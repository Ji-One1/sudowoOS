CC=gcc
CFLAGS=

mysh: shell.c interpreter.c shellmemory.c utils.c
	$(CC) $(CFLAGS) -c shell.c interpreter.c shellmemory.c utils.c
	$(CC) $(CFLAGS) -o mysh shell.o interpreter.o shellmemory.o utils.o

clean:
	rm mysh; rm *.o