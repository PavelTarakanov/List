#ifndef LIST_H
#define LIST_H

const int LEFT_BIRD = 2147483647;
const int START_LIST_SIZE = 10;
const int MAX_COMMAND_SIZE = 1000;

struct list_array_t {int data;
                     int next;
                     int prev;
                     int elem_status;};

struct list_t {list_array_t* list_array;
               int free_head;
               int list_len;
               int dump_number;
};

enum list_errors{
    NO_ERROR = 0,
    LIST_ADDRESS_ERROR = 1,
    LIST_LEN_ERROR = 2,
    LEFT_BIRD_ERROR = 3,
    ACCORDANCE_ERROR = 4,
    FILE_OPENING_ERROR = 5,
    FILE_CLOSING_ERROR = 6,
    ERROR_WHILE_MAKING_GRAPH = 7,
    ALLOCATION_ERROR = 8,
};

bool list_init(list_t* list, int list_size);
int list_upgrade(list_t* list);
void list_free(list_t* list);
int list_verify(list_t* list);
int list_dump(list_t* list, char* dump_file_name, FILE* html_dump_address);
int list_delete(list_t* list, int elem, char* dump_file_name, FILE* html_dump_address);
int list_insert(list_t* list, int value, int elem, char* dump_file_name, FILE* html_dump_address);

void start_html_dump(FILE* html_dump_address);

#endif //LIST_H
