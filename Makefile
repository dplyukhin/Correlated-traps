CFLAGS=-std=c99 -Wall -O3
EXEC=simulate
DEPS=*.h
LIBS=-pthread -lm
CC=gcc

all: simulate

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

simulate: cantor.o sg.o sgt.o rng.o simulate.o 
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)


# Set target-specific variable
debug: CFLAGS+=-g
debug: all

# Ignores any files actually called `clean'
.PHONY: clean
clean:
	rm -rf raw_data/ simulate *.o
