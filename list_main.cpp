#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "list.h"
#include "file_using.h"

const unsigned int NUMBER_OF_FILES = 2;

int main(int argc, char* argv[])
{
    list_t list = {};
    FILE* html_dump_address = NULL;

    if (check_file_founded(argc, NUMBER_OF_FILES))
        return 1;

    if (check_file_opening(argv[2], &html_dump_address, "w"))
        return 1;

    start_html_dump(html_dump_address);

    if (list_init(&list, START_LIST_SIZE))
        return 1;

    list_insert(&list, 10, 0, argv[1], html_dump_address);
    list_insert(&list, 20, 1, argv[1], html_dump_address);
    list_insert(&list, 30, 2, argv[1], html_dump_address);
    list_insert(&list, 40, 3, argv[1], html_dump_address);
    list_insert(&list, 50, 4, argv[1], html_dump_address);

    list_dump(&list, argv[1], html_dump_address);

    list_free(&list);

    return 0;
}
