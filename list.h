#ifndef LIST_H
#define LIST_H

const int LEFT_BIRD = 2147483647;
const int START_LIST_SIZE = 10;

struct list_t {int* data;
               int* next;
               int* prev;
               int* elem_status;
               int free_head;
               int list_len;
};

enum list_errors{
    NO_ERROR = 0,
    LIST_ADDRESS_ERROR = 1,
    LIST_LEN_ERROR = 2,
    LEFT_BIRD_ERROR = 3,
    ACCORDANCE_ERROR = 4,
};

bool list_init(list_t* list, int list_size);
void list_free(list_t* list);
int list_verify(list_t* list);
void list_dump(list_t* list, char* dump_file_name);
int list_delete(list_t* list, int elem, char* dump_file_name);
int list_insert(list_t* list, int value, int elem, char* dump_file_name);

#endif //LIST_H
