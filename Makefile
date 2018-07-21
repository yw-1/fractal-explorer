CC=gcc
CFLAGS=-I.
LIBS=-lm -lSDL2
DEPS=calc.h
OBJ=frac.o calc.o mset.o trfrac.o lzfrac.o lemon.o test.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
frac: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)
clean:
	rm -rf *.o
