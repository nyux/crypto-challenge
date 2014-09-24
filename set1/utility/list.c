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

void resize_array(list_t *list)
{
    void **arr;
    
    list->list_size *= 2;
    arr = calloc(list->list_size * 2, sizeof(void *));

    for (int i = 0; i < list->slots_used; i++) arr[i] = list->list[i];

    free(list->list);
    list->list = arr;
}

void list_add(list_t *list, void *obj)
{
    if (list->slots_used == list->list_size) resize_array(list); 

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
