#ifndef STRING_LIST_H
#define STRING_LIST_H

typedef struct node {
    char* value;
    struct node* next;
} entry_t;

entry_t* list_create(char* value);

void list_free(entry_t* head);

#endif