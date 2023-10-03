//
// Created by AboHolo on 26-Sep-23.
//

#include <bits/stdint-uintn.h>

#ifndef DOCUMENTTREE_MAIN_H
#define DOCUMENTTREE_MAIN_H

#endif //DOCUMENTTREE_MAIN_H

typedef enum {
    bool_t,
    int_t,
    string_t,
    struct_t,
    float_t,
} data_type;

typedef enum {
    FILE_STATUS_OK = 0,
    FILE_STATUS_ALREADY_EXISTS = 1,
    FILE_STATUS_NOT_EXIST = 2,
    FILE_STATUS_WRONG_FORMAT = 3,
    FILE_STATUS_UNABLE_OPEN = 4,
    FILE_STATUS_UNABLE_RELEASE = 5,
    FILE_STATUS_ERROR = 6,
} file_status;

typedef enum {
    SUCCESS,
    ERROR,
} operation_status;

struct allocator_t {
    FILE *file;

};

struct db_header_t {
    uint64_t key;
    enum data_type* types;
};

struct database_t {
    char* name;
    struct db_header_t* header;
    struct allocator_t* allocator;
};

struct record {

};

struct page_header {

};

struct page {
    struct page_header header;
    struct record* first_record;
    struct record* last_record;
};
