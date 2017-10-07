#ifndef _LIBBIRB_UNROLLED_LIST_H
#define _LIBBIRB_UNROLLED_LIST_H

#include <stdlib.h>

// MUST BE EVEN
#ifndef LIST_NODE_CAPACITY
#define LIST_NODE_CAPACITY 16
#endif

#ifndef LIST_DATA_TYPE
#define LIST_DATA_TYPE int
#endif

#define LIST_INDEX_OUT_OF_BOUNDS 3
#define LIST_NO_NODE_FOUND 2
#define LIST_ALLOCATION_FAILURE 1

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

struct unrolled_list *list_new(void);
void list_free(struct unrolled_list *list);

int list_insert(struct unrolled_list *list, size_t place, LIST_DATA_TYPE item);
int list_get(const struct unrolled_list *list, size_t place, LIST_DATA_TYPE *item);
int list_remove(struct unrolled_list *list, size_t place);

void list_print(struct unrolled_list *list);

static int insert_new_node(struct list_node *node);
static void split_nodes(struct list_node *src, struct list_node *dst);
static void node_shift_elements(struct unrolled_list *list, struct list_node *node, size_t index, int offset);
static LIST_DATA_TYPE *insert_point(struct unrolled_list *list, struct list_node *node, size_t place);
static struct list_node *find_node(const struct unrolled_list *list, size_t place, size_t *node_offset);

#endif
