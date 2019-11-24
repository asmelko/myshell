#ifndef STRING_LIST_H
#define STRING_LIST_H

#include <stddef.h>

typedef struct entry_node {
    char* value;
    struct entry_node* next;
} entry_t;

entry_t* list_create(char* value);

size_t list_size(const entry_t* head);

void list_free(entry_t* head);

#endif