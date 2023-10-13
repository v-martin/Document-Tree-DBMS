//
// Created by AboHolo on 26-Sep-23.
//

#include <bits/stdint-uintn.h>
#include <stdbool.h>

#ifndef DOCUMENTTREE_MAIN_H
#define DOCUMENTTREE_MAIN_H
#define MAX_STRING_LENGTH 1000
#define MAX_KEY_VALUES 100

typedef uint64_t offset_t;

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
} op_status;

//struct db_header_t {
//    uint64_t key;
//    enum data_type* types;

typedef struct page_header {
    offset_t last_record_offset;
} page_header_t;

typedef struct page_t {
    struct page_header *header;
    struct page_t *next;
    struct page_t *prev;
    offset_t first_record;
    offset_t last_record;
} page_t;

typedef struct allocator_t {
    FILE *file;
    struct page_t* page;
} allocator_t;

typedef struct database_t {
//    struct db_header_t* header;
    struct allocator_t* allocator;
} database_t;

typedef struct record_t {
    int64_t key;
    int64_t value;
} record_t;

//typedef struct record {
//    size_t num_key_values;
//    key_value_t key_values[MAX_KEY_VALUES];
//} record_t;

typedef struct key_value {
    char key[MAX_STRING_LENGTH];
    union {
        int64_t int_value;
        double float_value;
        char string_value[MAX_STRING_LENGTH];
        bool bool_value;
        record_t record;
    } value;
} key_value_t;


allocator_t *allocator_init(const char* filename);

op_status allocator_free(allocator_t *allocator);

database_t *db_init(const char *filename);

op_status db_free(database_t *database);

op_status create_page(allocator_t *allocator);

offset_t append_to_page(allocator_t *allocator, const record_t *record);

op_status write_record(allocator_t *allocator, record_t *record);

record_t *read_record(const allocator_t *allocator, size_t index);

op_status save_page(struct page_t *page, FILE *file);

#endif //DOCUMENTTREE_MAIN_H
