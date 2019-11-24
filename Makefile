CC = gcc
CFLAGS = -Wall -Wextra $D
_DEPS = line.h mysh.h string_list.h processor.h command.h scanner.h
_OBJ = source.o mysh.o line.o lexer.o string_list.o processor.o command.o
LIBS = -lfl -lreadline

DEPS = $(patsubst %, include/%,$(_DEPS))
OBJ = $(patsubst %, build/%,$(_OBJ))

.PHONY: clean all test-1 test-2

all: build generated mysh 

mysh: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

build/%.o: src/%.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

build/lexer.o: src/lexer.l 
	flex -o generated/lexer.c src/lexer.l
	$(CC) $(CFLAGS) -c -o $@ generated/lexer.c

build:
	mkdir build

generated:
	mkdir generated

test-1: all
	cd tests; \
	./run-tests.sh $$( cat phase-1.tests )

test-2: all	
	cd tests; \
	./run-tests.sh

clean: 
	rm -rf build generated mysh
