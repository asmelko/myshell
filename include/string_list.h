#ifndef STRING_LIST_H
#define STRING_LIST_H

#include <stddef.h>

//struct for node of linked list of strings
typedef struct entry_node {
	char* value;
	struct entry_node* next;
} entry_t;

//creates new node of string list
entry_t* list_create(char* value);

//gets size of string list
size_t list_size(const entry_t* head);

//frees whole list
void list_free(entry_t* head);

#endif