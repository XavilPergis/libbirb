#include "./src/unrolled_list.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    struct unrolled_list *list = list_new();
    if (!list)
        return 2;

    for (int i = 0; i < 32; i++)
    {
        for (int j = 0; j < i; j++)
        {
            int insert_index;
            if (list->count != 0)
                insert_index = rand() % list->count;
            else
                insert_index = 0;

            int status = list_insert(list, insert_index, i*j);
            if (status) {
                printf("Failed at index: %i, list->count = %zu\n", insert_index, list->count);
                printf("Status: %i\n", status);
                list_free(list);
                return 1;
            }
        }
    }

    list_print(list);

    for (int i = 0; i < list->count / 10; i += 10)
    {
        printf("list[%i] = %i\n", i, list_get(list, i, NULL));
    }

    int get_status = 0;
    list_get(list, 999, &get_status);

    return 0;
}
