#include "./unrolled_list.c"

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
            int status = list_insert(list, i, i*j);
            if (status) {
                printf("Failed at index: %i, list->count = %zu", insert_index, list->count);
                list_free(list);
                return 1;
            }
        }
    }

    printf("Program gets to here");

    list_print(list);

    return 0;
}
