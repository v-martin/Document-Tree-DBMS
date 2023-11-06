//
// Created by AboHolo on 26-Sep-23.
//

#ifndef DOCUMENTTREE_MAIN_H
#define DOCUMENTTREE_MAIN_H
#define MAX_KEY_VALUES 100

#include <stdint.h>
#include <stdbool.h>
#include "../buffer/buffer.h"
#include "../file_handler/file_handler.h"

typedef uint64_t offset_t;

//struct db_header_t {
//    uint64_t key;
//    enum data_type* types;

typedef enum {
    INT_VALUE,
    DOUBLE_VALUE,
    STRING_VALUE,
    BOOL_VALUE,
    STRUCT_VALUE
} value_type_t;

typedef struct{
    char *key;
    value_type_t value_type;
    union value_t{
        int64_t int_value;
        double float_value;
        char *string_value;
        bool bool_value;
        struct key_value_t *key_value;
    } value;
} key_value_t;

typedef struct{
    uint32_t size;
    uint32_t num_key_values;
    key_value_t key_values[MAX_KEY_VALUES];
} record_t;

typedef struct page_t {
    struct page_header *header;
    record_t *data[];
} page_t;

typedef struct page_header {
    offset_t last_record_offset;
    uint64_t record_size;
    page_t *next;
    page_t *prev;
} page_header_t;

typedef struct allocator_t {
    file_handler_t *file_handler;
    page_t* page;
} allocator_t;

typedef struct database_t {
//    struct db_header_t* header;
    allocator_t* allocator;
} database_t;


allocator_t *allocator_init(const char* filename);

op_status allocator_free(allocator_t *allocator);

void page_free(page_t *page);

void page_header_free(page_header_t *header);

database_t *db_init(const char *filename);

op_status db_free(database_t *database);

op_status create_page(allocator_t *allocator);

record_t *create_record(const key_value_t *key_values, size_t num_key_values);

void record_free(record_t *record);

op_status write_record(allocator_t *allocator, record_t *record);

record_t *read_record(allocator_t *allocator, size_t index);

op_status save_page(struct page_t *page, FILE *file);

buffer_t *bufferize_record(record_t *record);

#endif //DOCUMENTTREE_MAIN_H
