#include <stdio.h>
#include <stdlib.h>
#include "list.h"//спросить про стрелки и про system

int main(int argc, char* argv[])
{
    list_t list = {};

    if (check_file_founded(argc))
        return 1;

    if (list_init(&list, START_LIST_SIZE))
        return 1;

    list_insert(&list, 10, 0, argv);
    list_insert(&list, 20, 1, argv);
    list_insert(&list, 30, 2, argv);
    list_insert(&list, 40, 3, argv);
    list_insert(&list, 50, 4, argv);
    list_insert(&list, 45, 3, argv);
    list_delete(&list, 2, argv);

    list_dump(&list, argv);

    list_free(&list);

    return 0;
}

int list_insert(list_t* list, int value, int elem, char** argv)
{
    if (elem > list->list_len)
    {
        printf("Too big element number!\n");
        return 0;
    }
    else if (list->data[elem] == NO_ELEM)
    {
        printf("Can't insert after free element!\n");
        return 0;
    }

    if (list_verify(list))
        list_dump(list, argv);

    int free_number = list->free_head;

    list->free_head = list->next[free_number];
    list->data[free_number] = value;
    list->next[free_number] = list->next[elem];
    list->prev[free_number] = elem;
    list->next[elem] = free_number;
    list->prev[list->next[free_number]] = free_number;

    if (list_verify(list))
        list_dump(list, argv);

    return free_number;
}

int list_delete(list_t* list, int elem, char** argv)
{
    if (elem > list->list_len)
    {
        printf("Too big element number!\n");
        return 0;
    }
    else if (list->data[elem] == NO_ELEM)
    {
        printf("Can't delete free element!\n");
        return 0;
    }

    if (list_verify(list))
        list_dump(list, argv);

    list->data[elem] = NO_ELEM;
    list->next[list->prev[elem]] = list->next[elem];
    list->prev[list->next[elem]] = list->prev[elem];
    list->next[elem] = list->free_head;
    list->free_head = elem;

    if (list_verify(list))
        list_dump(list, argv);

    return 1;
}

void list_dump(list_t* list, char** argv)
{
    FILE* dump_address = NULL;

    if (check_file_opening(argv[1], &dump_address, "w"))
        return;

    /*printf("list->data[%p]\n", list->data);
    for (int i = 0; i < list->list_len; i++)
        printf("list->data[%d] = %d\n", i, list->data[i]);

    printf("list->next[%p]\n", list->next);
    for (int i = 0; i < list->list_len; i++)
        printf("list->next[%d] = %d\n", i, list->next[i]);

    printf("list->prev[%p]\n", list->prev);
    for (int i = 0; i < list->list_len; i++)
        printf("list->prev[%d] = %d\n", i, list->prev[i]);

    printf("list->free_head = %d\n"
           "list->list_len = %d\n", list->free_head, list->list_len);*/


    fprintf(dump_address, "digraph{\n"
                          "    rankdir=LR\n");
    for (int i = 0; i < list->list_len; i++)
    {
        if (list->data[i] == NO_ELEM)
            fprintf(dump_address, "    %d[color=\"red\", style=\"filled\",fillcolor=\"lightgrey\", shape = record, label=\"index = %d | value = %s | {prev = %d | next = %d}\"];\n",
                i, i, "NO_ELEM", list->prev[i], list->next[i]);
        else
            fprintf(dump_address, "    %d[color=\"green\", style=\"filled\",fillcolor=\"lightgrey\", shape = record, label=\"index = %d | value = %d | {prev = %d | next = %d}\"];\n",
                i, i, list->data[i], list->prev[i], list->next[i]);
        if (list->data[i] != NO_ELEM)
        {
            fprintf(dump_address, "    %d -> %d[color=\"blue\"]\n", i, list->next[i]);
            fprintf(dump_address, "    %d -> %d[color=\"red\"]\n", i, list->prev[i]);
        }
    }

    for (int i = list->free_head; i != 0; i = list->next[i])
    {
        fprintf(dump_address,"%d ", i);
        if (list->next[i] != 0)
            fprintf(dump_address, "-> ");
    }
    fprintf(dump_address, "[color = \"green\"]\n");

    fprintf(dump_address, "}");
    //system("dot dump.txt -T png -o dump.png");

    if (check_file_closing(dump_address))
        return;
    return;
}


bool list_init(list_t* list, int list_size)
{
    list->free_head = 1;
    list->list_len = list_size;
    list->data = (int*) calloc(list_size, sizeof(int));
    list->next = (int*) calloc(list_size, sizeof(int));
    list->prev = (int*) calloc(list_size, sizeof(int));
    if (list->data == NULL || list->next == NULL || list->prev == NULL)
        return true;
    list->data[0] = LEFT_BIRD;
    list->next[0] = 0;
    list->prev[0] = 0;
    for (int i = 1; i < list_size; i++)
    {
        list->data[i] = NO_ELEM;
        list->next[i] = i+1;
        list->prev[i] = -1;
    }
    list->next[list_size-1] = 0;

    return 0;
}

bool list_free(list_t* list)
{
    free(list->data);
    free(list->next);
    free(list->prev);

    return 0;
}

int list_verify(list_t* list)
{
    if (list == NULL)
        return LIST_ADDRESS_ERROR;

    if (list->list_len < 0)
        return LIST_LEN_ERROR;

    if (list->data[0] != LEFT_BIRD)
        return LEFT_BIRD_ERROR;

    for (int i = 0; i < list->list_len; i++)
    {
        if (list->prev[list->next[i]] != i)
            return ACCORDANCE_ERROR;

        if (list->next[list->prev[i]] != i)
            return ACCORDANCE_ERROR;
    }

    return NO_ERROR;
}
