#ifndef STRING_LIST_H
#define STRING_LIST_H

#include <stddef.h>

typedef struct node {
    char* value;
    struct node* next;
} entry_t;

entry_t* list_create(char* value);

size_t list_size(const entry_t* head);

void list_free(entry_t* head);

#endif