CC=gcc
CFLAGS=-I.
LIBS=-lm -lSDL2
#DEPS=frac.h
OBJ=frac.o mset.o trfrac.o lzfrac.o lemon.o test.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
frac: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)
clean:
	rm -rf *.o
.PHONY: clean
