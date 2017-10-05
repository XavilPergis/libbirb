#ifndef _LIBBIRB_UNROLLED_LIST_H
#define _LIBBIRB_UNROLLED_LIST_H

// MUST BE EVEN
#ifndef LIST_NODE_CAPACITY
#define LIST_NODE_CAPACITY 16
#endif

#ifndef LIST_DATA_TYPE
#define LIST_DATA_TYPE int
#endif

struct unrolled_list
{
    struct list_node *head;
    size_t count;
};

struct list_node
{
    struct list_node *next;
    size_t count;

    LIST_DATA_TYPE data[LIST_NODE_CAPACITY];
};

int list_new(struct unrolled_list *list);
void list_free(struct unrolled_list *list);

int list_insert(struct unrolled_list *list, size_t place, LIST_DATA_TYPE item);
LIST_DATA_TYPE list_get(struct unrolled_list *list, size_t place);
void list_remove(struct unrolled_list *list, size_t place);

void list_print(struct unrolled_list *list);

#endif