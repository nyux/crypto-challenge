#ifndef LIST

#define LIST

typedef struct
{
    size_t list_size;
    size_t slots_used;
    void **list;
} list_t;

list_t* list_new();
void list_add(list_t *list, void *obj);
size_t list_size(list_t *list);
void* list_get(list_t *list, size_t index);
void list_empty(list_t *list);
void list_free(list_t **list);

#endif
