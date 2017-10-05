#include "./unrolled_list.h"

int main(int argc, char **argv)
{
    struct unrolled_list *list = list_new();

    for (int i = 0; i < 32; i++) {
        for (int j = 0; j < i; j++) {
            int insert_index = rand() % 
            printf("Inserting %i at index %i\n", i*j, i);
            int status = list_insert(list, i, i*j);
            printf("STATUS: %i\n", status);
            if (status) return 1;
        }
    }

    printf("\n\nEND STATE:\n");
    debug_print_node(list);
    debug_print_node(list->next);

    list_print(list);
}
