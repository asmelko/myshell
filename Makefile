CC=gcc
FLAGS=-Wall -Wextra -Iinclude
CFLAGS= $(FLAGS) $(EXT)
DEPS=include/line.h include/mysh.h

.PHONY: clean all

all: build mysh

mysh: src/source.c build/mysh.o build/line.o 
	$(CC) $(CFLAGS) -o $@ $^

build/%.o: src/%.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<
	

build:
	mkdir build

clean: 
	rm -f mysh
	rm -rf build
