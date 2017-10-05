#include "./unrolled_list.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

struct unrolled_list *list_new(void)
{
    struct unrolled_list *list = malloc(sizeof(struct unrolled_list));
    if (!list)
        return NULL;

    struct list_node *head = malloc(sizeof(struct list_node));
    if (!head)
        return NULL;

    head->next = NULL;
    head->count = 0;
    list->count = 0;
    list->head = head;

    return list;
}

void list_free(struct unrolled_list *list)
{
    struct list_node *current = list->head;

    while (current)
    {
        // Need to make sure that we don't free the memory from under us and
        // *then* try to get the next node
        struct list_node *next = current->next;
        free(current);
        current = next;
    }

    // Then free the container
    free(list);
}

int list_insert(struct unrolled_list *list, size_t place, LIST_DATA_TYPE item)
{
    size_t node_offset = 0;

    // Find the node the index we want is in
    struct list_node *node = find_node(list, place, &node_offset);
    if (!node)
        return 2;

    // Get the point of insertion, shifting elements and allocating new nodes if need be
    LIST_DATA_TYPE *insert_at = insert_point(node, node_offset);
    if (!insert_point)
        return 1;

    *insert_at = item;
    return 0;
}

LIST_DATA_TYPE list_get(const struct unrolled_list *list, size_t place)
{
    size_t node_offset = 0;

    // Find the right node
    struct list_node *node = find_node(list, place, &node_offset);
    if (!node)
        return 2;
    
    // Instead of setting an item here, we just pull it out of the node's array
    return node->data[node_offset];
}

static void print_seq(int *data, size_t length)
{
    printf("[");
    for (size_t i = 0; i < length; i++)
    {
        if (i == length - 1)
            printf("%i", data[i]);
        else
            printf("%i ", data[i]);
    }
    printf("]\n");
}

static void debug_print_node(struct list_node *node)
{
    if (!node)
        return;

    printf("%p: <next=%p, count=%zu, data=[", node, node->next, node->count);
    print_seq(node->data, LIST_NODE_CAPACITY);
    printf("]>\nActual: ");
    print_seq(node->data, node->count);
}

void list_print(struct unrolled_list *list)
{
    if (!list)
        return;
    struct list_node *node = list->head;
    printf("[");
    while (node)
    {
        for (size_t i = 0; i < node->count; i++)
        {
            if (!node->next && i == node->count - 1)
                printf("%i", node->data[i]);
            else
                printf("%i, ", node->data[i]);
        }
        node = node->next;
    }
    printf("]\n");
}

static int insert_new_node(struct list_node *node)
{
    struct list_node *new_node = malloc(sizeof(struct list_node));
    if (!new_node)
        return 1;

    // We don't need to initialized node->data
    new_node->count = 0;
    new_node->next = node->next;
    node->next = new_node;

    return 0;
}

// @PRE: DST must be a new node (no elements)
static void split_nodes(struct list_node *src, struct list_node *dst)
{
    memcpy(dst->data, &src->data[LIST_NODE_CAPACITY / 2], sizeof(LIST_DATA_TYPE) * LIST_NODE_CAPACITY / 2);
    src->count = LIST_NODE_CAPACITY / 2;
    dst->count = LIST_NODE_CAPACITY / 2;
}

// @PRE: NODE must not be at capacity
// @PRE: OFFSET must be less than or equal to NODE's count
static void node_shift_elements(struct list_node *node, size_t offset)
{
    assert(offset + 1 <= LIST_NODE_CAPACITY);
    memmove(&node->data[offset + 1], &node->data[offset], sizeof(LIST_DATA_TYPE) * node->count);
    node->count++;
}

static LIST_DATA_TYPE *insert_point(struct list_node *node, size_t place)
{
    if (!node)
        return NULL;

    if (node->count != LIST_NODE_CAPACITY)
    {   
        // We still have space in node->data, so we can just shift the contents over one
        node_shift_elements(node, place);
        return &node->data[place];
    }
    else
    {
        int in_first_half = place <= LIST_NODE_CAPACITY / 2;

        // We can use node->next because we always populate it here
        int status = insert_new_node(node);
        if (!status)
            return NULL;
        
        // Copy the top half of this node into the bottom of the next
        split_nodes(node, node->next);

        // Since the data is now split across two nodes, we have to check which node our
        // data needs to be inserted into
        if (in_first_half)
        {
            // The bottom half stays aligned to 0, so PLACE still points where we need it to
            node_shift_elements(node, place);
            return &node->data[place];
        }
        else
        {
            // PLACE is now half a node too high, since we shifted data starting
            // from the half-way point back to 0, so we need to compensate for the shift
            size_t new_place = place - (LIST_NODE_CAPACITY / 2);
            node_shift_elements(node->next, new_place);
            return &node->next->data[new_place];
        }
    }
}

// @PRE: PLACE must be an element within the list, or at the beginning/end
// @POST: OBSERVED_COUNT will be aligned to the beginning of the node
static struct list_node *find_node(const struct unrolled_list *list, size_t place, size_t *node_offset)
{
    size_t total = 0;
    if (!list)
        return NULL;
    struct list_node *current = list->head;
    while (1)
    {
        // If the index is inside the range for a node, return that node
        // Otherwise, add the count to the total visited and crawl the list
        if (total <= place && place <= total + current->count)
        {
            *node_offset = place - total;
            return current;
        }
        else
        {
            // Oh no! we've reached the end of the list without ever finding the node!
            if (!current->next)
                return NULL;
            total += current->count;
            current = current->next;
        }
    }
}
