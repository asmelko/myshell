#include "../include/string_list.h"

#include <stdlib.h>
#include <err.h>

entry_t* list_create(char* value)
{
    entry_t* head = (entry_t*)malloc(sizeof(entry_t));
    if(head == NULL)
        err(1,"list_create");
    head->value = value;
    head->next = NULL;
    return head;
}

void list_free(entry_t* head)
{
    while(head) {
        entry_t* tmp = head->next;
        free(head->value);
        free(head);
        head = tmp;
    }
}
