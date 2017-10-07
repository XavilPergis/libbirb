#include "./src/unrolled_list.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    struct unrolled_list *list = list_new();
    if (!list)
        return 2;

    for (int i = 0; i < 100; i++)
    {
        int insert_index;
        if (list->count != 0)
            insert_index = rand() % list->count;
        else
            insert_index = 0;

        int status = list_insert(list, insert_index, i);

        if (status) {
            printf("Failed at index: %i, list->count = %zu\n", insert_index, list->count);
            printf("Status: %i\n", status);
            list_free(list);
            return 1;
        }
    }

    printf("\n\nLIST:\n\n");
    list_print(list);
    printf("\n\n");

    // for (struct list_node *node = list->head; node->next; node = node->next)
    // {
    //     for (int i = 0; i < node->count; i++)
    //     {

    //     }
    // }

    for (int i = list->count; i > 0; i--)
    {
        int index;
        if (list->count != 0)
            index = rand() % list->count;
        else
            index = 0;

        int status = list_remove(list, index);
        if (status)
        {
            printf("Failed at index: %i, list->count = %zu\n", index, list->count);
            printf("Status: %i\n", status);
            
            debug_print_list(list);
            
            list_free(list);
            return 1;
        }
    }
    
    list_print(list);

    int remove_status = list_remove(list, 99999);
    printf("remove_status: %i\n", remove_status);
    
    for (int i = 0; i < list->count / 10; i += 10)
    {
        int item = 0;
        int status = list_get(list, i, &item);
        printf("list[%i] = %i\n", i, item);
    }

    int get_item = 0;
    int get_status = list_get(list, 999, &get_status);

    printf("get_item=%i (should be 0), get_status=%i (should be 3)\n", get_item, get_status);

    list_free(list);

    return 0;
}
