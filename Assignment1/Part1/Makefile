CC=gcc
OBJECTS = main.o
DEPS = vinegere.h
CFLAGS=-DDEBUG

part1 : $(OBJECTS)
	$(CC) -o part1.out $^

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< 

debug : maindebug.o vinegeredebug.o debug.o
	$(CC) -o debug.out $^ $(CFLAGS)

maindebug.o: main.c  $(DEPS) debug.h
	$(CC) -c -o $@ $< $(CFLAGS)

vinegeredebug.o: vinegere.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

debug.o: debug.c $(DEPS) debug.h
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -rf *o core
