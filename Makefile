CC=gcc
FLAGS=-Wall -Wextra

mysh: src/mysh.c
	@$(CC) $(FLAGS) -o mysh src/mysh.c

.PHONY: clean all

clean: 
	@rm -f mysh

all: clean mysh