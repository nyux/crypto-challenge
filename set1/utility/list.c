#include <stdlib.h>

#include "list.h"
#include "utility.h"

list_t* list_new()
{
    list_t *list = malloc(sizeof(list_t));

    if (!list) utility_malloc_error();
    
    list->size = 10;
    list->slots_used = 0;
    list->list = calloc(list->size, sizeof(void *));

    if (!list->list) utility_malloc_error();

    return list;
}

void list_add(list_t *list, void *obj)
{
    if (list->slots_used == list->size)
    {
        list->size *= 2;
        list->list = realloc(list->list, list->size * sizeof(void *));
    }

    list->list[list->slots_used] = obj;
    list->slots_used++;
}



size_t list_size(list_t *list)
{
    return list->slots_used;
}

void* list_get(list_t *list, size_t index)
{
    return (index > list->slots_used) ? NULL : list->list[index];
}

void list_empty(list_t *list)
{
    for (int i = 0; i < list->slots_used; i++) list->list[i] = NULL;
}

void list_free(list_t **list)
{
    free((*list)->list);
    free(*list);
}
