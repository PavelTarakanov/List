#ifndef LIST_H
#define LIST_H

const int LEFT_BIRD = 2147483647;
const int START_LIST_SIZE = 10;
const int MAX_COMMAND_SIZE = 1000;

struct list_array_t {int data;
                     unsigned int next;
                     unsigned int prev;
                     bool elem_status;};

struct list_t {list_array_t* list_array;
               int free_head;
               unsigned int list_len;
               unsigned int dump_number;
};

enum list_errors{
    NO_ERROR = 0,
    LIST_ADDRESS_ERROR = 1,
    LEFT_BIRD_ERROR = 2,
    ACCORDANCE_ERROR = 3,
    FILE_OPENING_ERROR = 4,
    FILE_CLOSING_ERROR = 5,
    ERROR_WHILE_MAKING_GRAPH = 6,
    ALLOCATION_ERROR = 7,
    COMMAND_MAKING_ERROR = 8,
};

bool list_init(list_t* list, unsigned int list_size);
int list_upgrade(list_t* list);
void list_free(list_t* list);
int list_verify(list_t* list);
int list_dump(list_t* list, char* const dump_file_name, FILE* const html_dump_address);
int list_delete(list_t* list, unsigned int elem, char* const dump_file_name, FILE* const html_dump_address);
int list_insert(list_t* list, int value, unsigned int elem, char* const dump_file_name, FILE* const html_dump_address);
void start_html_dump(FILE* html_dump_address);

bool check_file_founded(int argc, int number_of_files);

#endif //LIST_H
