CC = gcc
CFLAGS = -Wall -Wextra $D
_DEPS = line.h mysh.h string_list.h processor.h command.h scanner.h
_OBJ = source.o mysh.o line.o lexer.o string_list.o processor.o command.o
LIBS = -lfl -lreadline

DEPS = $(patsubst %, include/%,$(_DEPS))
OBJ = $(patsubst %, build/%,$(_OBJ))

.PHONY: clean all test

all: build generated mysh

mysh: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

build/%.o: src/%.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

build/lexer.o: src/lexer.l 
	flex -o generated/lexer.c src/lexer.l
	$(CC) $(CFLAGS) -c -o $@ generated/lexer.c

generated/parser.%: src/parser.y
	bison --defines=generated/parser.h -o generated/parser.c src/parser.y
	
build/parser.o: generated/parser.c
	$(CC) $(CFLAGS) -c -o $@ $<

build:
	mkdir build

generated:
	mkdir generated

clean: 
	rm -rf build generated mysh
