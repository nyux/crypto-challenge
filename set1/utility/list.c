#include <stdlib.h>

#include "list.h"
#include "utility.h"

list_t* list_new()
{
    list_t *list = malloc(sizeof(list_t));

    if (!list) utility_malloc_error();
    
    list->list_size = 10;
    list->slots_used = 0;
    list->list = calloc(list->list_size, sizeof(void *));

    if (!list->list) utility_malloc_error();

    return list;
}

void list_add(list_t *list, void *obj)
{
    size_t index = list->slots_used;
    list->list[index] = obj;
    list->slots_used++;
}

size_t list_size(list_t *list)
{
    return list->list_size;
}

void* list_get(list_t *list, size_t index)
{
    return list->list[index];
}
