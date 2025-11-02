#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "list.h"
#include "file_using.h"

int main(int argc, char* argv[])
{
    list_t list = {};
    int dump_number = 0;

    if (check_file_founded(argc))
        return 1;

    if (list_init(&list, START_LIST_SIZE))
        return 1;

    list_insert(&list, 10, 0, argv[1], &dump_number);
    list_insert(&list, 20, 1, argv[1], &dump_number);
    list_insert(&list, 30, 2, argv[1], &dump_number);
    list_insert(&list, 40, 3, argv[1], &dump_number);
    list_insert(&list, 50, 4, argv[1], &dump_number);
    list_insert(&list, 45, 3, argv[1], &dump_number);
    list_delete(&list, 2, argv[1], &dump_number);

    list_dump(&list, argv[1], &dump_number);

    list_free(&list);

    return 0;
}

int list_insert(list_t* list, int value, int elem, char* dump_file_name, int* dump_number)
{
    assert(list);
    assert(dump_file_name);
    assert(dump_number);

    if (elem > list->list_len)
    {
        printf("Too big element number!\n");
        return 0;
    }
    else if (list->list_array[elem].elem_status == 0)
    {
        printf("Can't insert after free element!\n");
        return 0;
    }

    list_dump(list, dump_file_name, dump_number);

    int free_number = list->free_head;

    list->free_head = list->list_array[free_number].next;
    list->list_array[free_number].data = value;
    list->list_array[free_number].next = list->list_array[elem].next;
    list->list_array[free_number].prev = elem;
    list->list_array[free_number].elem_status = 1;
    list->list_array[elem].next = free_number;
    list->list_array[list->list_array[free_number].next].prev = free_number;

    list_dump(list, dump_file_name, dump_number);

    return free_number;
}

int list_delete(list_t* list, int elem, char* dump_file_name, int* dump_number)
{
    assert(list);
    assert(dump_file_name);
    assert(dump_number);

    if (elem > list->list_len)
    {
        printf("Too big element number!\n");
        return 0;
    }
    else if (list->list_array[elem].elem_status == 0)
    {
        printf("Can't delete free element!\n");
        return 0;
    }

    list_dump(list, dump_file_name, dump_number);

    list->list_array[elem].elem_status = 0;
    list->list_array[list->list_array[elem].prev].next = list->list_array[elem].next;
    list->list_array[list->list_array[elem].next].prev = list->list_array[elem].prev;
    list->list_array[elem].next = list->free_head;
    list->list_array[elem].elem_status = 0;
    list->free_head = elem;

    list_dump(list, dump_file_name, dump_number);

    return 1;
}

void list_dump(list_t* list, char* dump_file_name, int* dump_number)
{
    assert(list);
    assert(dump_file_name);
    assert(dump_number);

    FILE* dump_address = NULL;
    char* str_dump_number = (char*) calloc(1000, sizeof(char));
    char* command = (char*) calloc(1000, sizeof(char));

    command = strcpy(command, "dot dump.txt -T png -o dump.png");
    str_dump_number = do_str_number(str_dump_number, *dump_number);

    if (check_file_opening(dump_file_name, &dump_address, "w"))
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
           "list->list_len = %d\n", list->free_head, list->list_len);
    */
    fprintf(dump_address, "digraph{\n"
                          "\trankdir=LR\n");
    for (int i = 0; i < list->list_len; i++)
    {
        if (list->list_array[i].elem_status == 0)
            fprintf(dump_address, "\t%d[color=\"black\", style=\"filled\",fillcolor=\"lightgrey\", shape = record, label=\"index = %d | value = %s | {prev = %d | next = %d}\"];\n",
                i, i, "NO_ELEM", list->list_array[i].prev, list->list_array[i].next);
        else
            fprintf(dump_address, " \t%d[color=\"green\", style=\"filled\",fillcolor=\"lightgrey\", shape = record, label=\"index = %d | value = %d | {prev = %d | next = %d}\"];\n",
                i, i, list->list_array[i].data, list->list_array[i].prev, list->list_array[i].next);
        if (list->list_array[i].elem_status == 1)
        {
            fprintf(dump_address, "\t%d -> %d[color=\"blue\"]\n", i, list->list_array[i].next);
            fprintf(dump_address, "\t%d -> %d[color=\"red\"]\n", i, list->list_array[i].prev);
        }
    }

    for (int i = list->free_head; i != 0; i = list->list_array[i].next)
    {
        fprintf(dump_address,"%d ", i);
        if (list->list_array[i].next != 0)
            fprintf(dump_address, "-> ");
    }
    fprintf(dump_address, "[color = \"green\"]\n");

    fprintf(dump_address, "}");

    if (check_file_closing(dump_address))
        return;

    str_dump_number = strcat(str_dump_number, ".png");//TODO strncat
    command = strcat(command, str_dump_number);
    system(command);//TODO check return system

    free(str_dump_number);
    free(command);

    (*dump_number)++;

    return;
}

bool list_init(list_t* list, int list_size)//TODO dump_number in structure???
{
    assert(list);

    list->free_head = 1;
    list->list_len = list_size;
    list->list_array = (list_array_t*) calloc(list_size, sizeof(list_array_t));

    if (list->list_array == NULL)
        return true;

    list->list_array[0].data = LEFT_BIRD;
    list->list_array[0].next = 0;
    list->list_array[0].prev = 0;
    list->list_array[0].elem_status = 1;

    for (int i = 1; i < list_size; i++)
    {
        list->list_array[i].data = 0;
        list->list_array[i].next = i+1;
        list->list_array[i].prev = -1;
        list->list_array[i].elem_status = 0;
    }

    list->list_array[list_size-1].next = 0;

    return 0;
}

void list_free(list_t* list)
{
    free(list->list_array);

    return;
}

int list_verify(list_t* list)
{
    if (list == NULL)
        return LIST_ADDRESS_ERROR;

    if (list->list_len < 0)
        return LIST_LEN_ERROR;

    if (list->list_array[0].data != LEFT_BIRD)
        return LEFT_BIRD_ERROR;

    for (int i = 0; i < list->list_len; i++)
    {
        if (list->list_array[list->list_array[i].next].prev != i)
            return ACCORDANCE_ERROR;

        if (list->list_array[list->list_array[i].prev].next != i)
            return ACCORDANCE_ERROR;
    }

    return NO_ERROR;
}

char* do_str_number(char* str, int number)//TODO snprintf
{
    int i = 0;
    char reverse_str[10] = {0};

    //printf("number = %d\n", number);

    while (number > 9)
    {
        reverse_str[i] = (char) (number % 10) + '0';
        number = number/10;
        i++;
    }
    reverse_str[i] = (char) number + '0';

    //printf("reverse_str = %s\n", reverse_str);

    for (int j = i; j >= 0; j--)
    {
        str[i-j] = reverse_str[j];
    }
    str[i+1] = '\0';
    //printf("str = %s\n", str);
    return str;
}

