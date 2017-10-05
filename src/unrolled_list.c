#include "./unrolled_list.h"

int list_new(struct unrolled_list *list)
{
    struct list_node *head = malloc(sizeof(struct list_node));
    if (!list)
        return 1;

    head->next = NULL;
    head->count = 0;
    list->count = 0;
    list->head = head;

    return 0;
}

void list_free(struct unrolled_list *list)
{
    struct list_node *current = list->head;

    while (current)
    {
        struct list_node *next = current->next;
        free(current);
        current = next;
    }
}

int list_insert(struct unrolled_list *list, size_t place, LIST_DATA_TYPE item)
{
    size_t prev_nodes_count = 0;

    // Find the node the index we want is in
    struct list_node *node = find_node(list, place, &prev_nodes_count);
    if (!node)
        return 2;

    // Get the point of insertion, shifting elements and allocating new nodes if need be
    LIST_DATA_TYPE *insert_at = insert_point(node, place - prev_nodes_count);
    if (!insert_point)
        return 1;

    *insert_at = item;
    return 0;
}

LIST_DATA_TYPE list_get(struct unrolled_list *list, size_t place);
void list_remove(struct unrolled_list *list, size_t place);

static void debug_print_node(struct list_node *node)
{
    if (!node)
        return;

    printf("%p: <next=%p, count=%zu, data=[", node, node->next, node->count);
    print_seq(node->data, LIST_NODE_CAPACITY);
    printf("]>\nActual: ");
    print_seq(node->data, node->count);
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

void list_print(struct unrolled_list *list)
{
    if (!list)
        return;
    struct list_node *node = list;
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

    // leave data uninitialized
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
        split_nodes(node, node->next);

        if (in_first_half)
        {
            node_shift_elements(node, place);
            return &node->data[place];
        }
        else
        {
            assert(place >= LIST_NODE_CAPACITY / 2);
            size_t new_place = place - (LIST_NODE_CAPACITY / 2);
            node_shift_elements(node->next, new_place);
            return &node->next->data[new_place];
        }
    }
}

// @PRE: PLACE must be an element within the list, or at the beginning/end
// @POST: OBSERVED_COUNT will be aligned to entire nodes
static struct list_node *find_node(struct unrolled_list *list, size_t place, size_t *observed_count)
{
    size_t total = 0;
    if (!list)
        return NULL;
    struct list_node *current = list;
    while (1)
    {
        if (total <= place && place <= total + current->count)
        {
            *observed_count = total;
            return current;
        }
        else
        {
            if (!current->next)
                return NULL;
            total += current->count;
            current = current->next;
        }
    }
}
