#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "list.h"
#include "file_using.h"

int list_insert(list_t* list, int value, int elem, char* dump_file_name, FILE* html_dump_address)
{
    assert(list);
    assert(dump_file_name);

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

    fprintf(html_dump_address, "<h3> DUMP <font color=red> BEFORE </font> INSERT %d AFTER NUMBER %d</h3>",
            value, elem);

    if (list_dump(list, dump_file_name, html_dump_address))
        return -1;

    if (list->list_array[list->free_head].next == 0)
        if (list_upgrade(list))
            return -1;

    int free_number = list->free_head;

    list->free_head = list->list_array[free_number].next;
    list->list_array[free_number].data = value;
    list->list_array[free_number].next = list->list_array[elem].next;
    list->list_array[free_number].prev = elem;
    list->list_array[free_number].elem_status = 1;
    list->list_array[elem].next = free_number;
    list->list_array[list->list_array[free_number].next].prev = free_number;

    fprintf(html_dump_address, "<h3> DUMP <font color=red> AFTER </font> INSERT %d AFTER NUMBER %d</h3>",
            value, elem);

    if (list_dump(list, dump_file_name, html_dump_address))
        return -1;

    if (list->list_array[list->free_head].next == 0)
        if (list_upgrade(list))
            return -1;

    return free_number;
}

int list_delete(list_t* list, int elem, char* dump_file_name, FILE* html_dump_address)
{
    assert(list);
    assert(dump_file_name);

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

    fprintf(html_dump_address, "<h3> DUMP <font color=red> BEFORE </font> DELETE NUMBER %d</h3>",
            elem);

    if (list_dump(list, dump_file_name, html_dump_address))
        return -1;

    list->list_array[elem].elem_status = 0;
    list->list_array[list->list_array[elem].prev].next = list->list_array[elem].next;
    list->list_array[list->list_array[elem].next].prev = list->list_array[elem].prev;
    list->list_array[elem].next = list->free_head;
    list->list_array[elem].elem_status = 0;
    list->free_head = elem;

    fprintf(html_dump_address, "<h3> DUMP <font color=red> AFTER </font> DELETE NUMBER %d</h3>",
            elem);

    if (list_dump(list, dump_file_name, html_dump_address))
        return -1;

    return 1;
}

int list_dump(list_t* list, char* dump_file_name, FILE* html_dump_address)
{
    assert(list);
    assert(dump_file_name);

    FILE* dump_address = NULL;
    char* str_dump_number = NULL;
    str_dump_number = (char*) calloc(MAX_COMMAND_SIZE, sizeof(char));
    char* command = NULL;
    command = (char*) calloc(MAX_COMMAND_SIZE, sizeof(char));

    if (command == NULL || str_dump_number == NULL)
        return ALLOCATION_ERROR;


    sprintf(command, "%s%d%s", "dot dump.txt -T png -o dump_", list->dump_number, ".png");

    if (check_file_opening(dump_file_name, &dump_address, "w"))
        return FILE_OPENING_ERROR;

    fprintf(html_dump_address, "list->array[%p]\n"
                               "data:\t"
                               "%10s\t", list->list_array, "CANARY");
    for (int i = 1; i < list->list_len; i++)
    {
        if (list->list_array[i].elem_status == 1)
            fprintf(html_dump_address, "%10d\t", list->list_array[i].data);
        else
            fprintf(html_dump_address, "%10s\t", "NO_ELEM");
    }
    fprintf(html_dump_address, "\n"
                               "next:\t");

    for (int i = 0; i < list->list_len; i++)
        fprintf(html_dump_address, "%10d\t", list->list_array[i].next);

    fprintf(html_dump_address, "\n"
                               "prev:\t");
    for (int i = 0; i < list->list_len; i++)
        fprintf(html_dump_address, "%10d\t", list->list_array[i].prev);

    fprintf(html_dump_address, "\nlist->free_head = %d\n"
           "list->list_len = %d\n", list->free_head, list->list_len);

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
        return FILE_CLOSING_ERROR;

    if (system(command) != 0)
        return ERROR_WHILE_MAKING_GRAPH;

    fprintf(html_dump_address, "IMAGE:\n"
                              "<img src=dump_%d.png width = 700px>\n\n\n", list->dump_number);

    free(str_dump_number);
    free(command);

    list->dump_number++;

    return NO_ERROR;
}

bool list_init(list_t* list, int list_size)
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

int list_upgrade(list_t* list)
{
    assert(list);

    int new_list_len = list->list_len * 2;

    list->list_array = (list_array_t*) realloc(list->list_array, sizeof(list_array_t)*new_list_len);

    for (int i = list->list_len; i < new_list_len; i++)
    {
        list->list_array[i].data = 0;
        list->list_array[i].next = i+1;
        list->list_array[i].prev = -1;
        list->list_array[i].elem_status = 0;
    }

    list->list_array[list->free_head].next = list->list_len;
    list->list_array[new_list_len-1].next = 0;
    list->list_len = new_list_len;
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

void start_html_dump(FILE* html_dump_address)
{
    fprintf(html_dump_address, "<pre>\n"
                                "<h1>Dumps</h1>");
}
