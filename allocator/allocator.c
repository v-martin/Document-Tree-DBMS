//
// Created by AboHolo on 11-Oct-23.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "allocator.h"

#define PAGE_SIZE 4096
#define RECORD_SIZE 32

allocator_t *allocator_init(const char* filename) {
    struct allocator_t* allocator = malloc(sizeof(allocator_t));
    if (allocator == NULL) {
        perror("");
        return NULL;
    }
    file_handler_t *file_handler = file_handler_init(filename);
    if (file_handler == NULL) {
        free(allocator);
        return NULL;
    }
    allocator->file_handler = file_handler;
    op_status st = create_page(allocator);
    if (st == ERROR) {
        free(allocator);
        return NULL;
    }
    return allocator;
}

void page_header_free(page_header_t *header) {
    assert(header != NULL);
    header->next = NULL;
    header->prev = NULL;
    header->record_size = 0;
    header->last_record_offset = 0;
    free(header);
}

void page_free(page_t *page) {
    assert(page != NULL);
//    page_header_free(page->header);
    free(page);
}

op_status allocator_free(allocator_t *allocator) {
    assert(allocator != NULL);
    assert(allocator->file_handler != NULL);
    op_status st = file_handler_free(allocator->file_handler);

    if (st == ERROR) {
        return ERROR;
    }

    page_free(allocator->page);

    free(allocator);

    return SUCCESS;
}

database_t *db_init(const char *filename) {
    assert(filename != NULL);
    database_t *database = malloc(sizeof(struct database_t));
    if (database == NULL) {
        return NULL;
    }
    database->allocator = allocator_init(filename);
    if (database->allocator == NULL){
        allocator_free(database->allocator);
        free(database);
        return NULL;
    }
    return database;
}

op_status db_free(database_t *database) {
    assert(database != NULL);
    op_status status = allocator_free(database->allocator);
    if (status == ERROR){
        free(database);
        return ERROR;
    }
    free(database);
    return SUCCESS;
}

op_status create_page_header(page_t *page, uint64_t record_size, page_t *prev, page_t *next) {
    page_header_t *header = malloc(sizeof(page_header_t));
    if (header == NULL) {
        free(header);
        return ERROR;
    }
    header->last_record_offset = 0;
    header->record_size = record_size;
    header->prev = prev;
    header->next = next;
    page->header = header;
    return SUCCESS;
}

op_status create_page(allocator_t *allocator) {
    page_t *page = malloc(sizeof(page_t));
    op_status st_header = create_page_header(page, RECORD_SIZE, allocator->page, NULL);
    if (st_header == ERROR) {
        free(page);
        return ERROR;
    }

    if (allocator->page != NULL) {
        allocator->page->header->next = page;
    }
    allocator->page = page;
    return SUCCESS;
}

key_value_t *alloc_key_value(char *key, const value_type_t type, const void *value) {
    key_value_t *key_value = malloc(sizeof(key_value_t));
    if (key_value == NULL) {
        return NULL;
    }
    memcpy(&key_value->key, &key, sizeof(char *));
    memcpy(&key_value->value_type, &type, sizeof(value_type_t));

    switch (type) {
        case INT_VALUE:
            memcpy(&key_value->value.int_value, value, sizeof(int64_t));
            break;
        case DOUBLE_VALUE:
            memcpy(&key_value->value.float_value, value, sizeof(double ));
            break;
        case STRING_VALUE:
            memcpy(&key_value->value.bool_value, value, sizeof(bool));
            break;
        case BOOL_VALUE:
            memcpy(&key_value->value.string_value, &value, sizeof(char *));
            break;
        case STRUCT_VALUE:
            memcpy(&key_value->value.key_value, &value, sizeof(key_value_t *));
            break;
    }

    return key_value;
}

uint32_t get_record_size(const key_value_t *key_values, size_t num_key_values) {
    uint32_t total_size = sizeof(uint32_t) * 2;

    for (size_t i = 0; i < num_key_values; i++) {
        total_size += strlen(key_values[i].key) + 1;
        total_size += sizeof(value_type_t);
        switch (key_values[i].value_type) {
            case INT_VALUE:
                total_size += sizeof(int64_t);
                break;
            case DOUBLE_VALUE:
                total_size += sizeof(double);
                break;
            case STRING_VALUE:
                total_size += strlen(key_values[i].value.string_value) + 1;
                break;
            case BOOL_VALUE:
                total_size += sizeof(bool);
                break;
            case STRUCT_VALUE:
                total_size += sizeof(key_value_t);
                break;
        }
    }
    return total_size;
}

record_t *create_record(const key_value_t *key_values, size_t num_key_values) {
    uint32_t total_size = get_record_size(key_values, num_key_values);

    record_t *record = malloc(total_size);
    memcpy(record, &total_size, sizeof(uint32_t));
    memcpy(&(record->num_key_values), &num_key_values, sizeof(uint32_t));

    for (size_t i = 0; i < num_key_values; i++) {
        char *key = key_values[i].key;
        value_type_t type = key_values[i].value_type;
        union value_t value = key_values[i].value;
        memcpy(&record->key_values[i], alloc_key_value(key, type, &value), sizeof(key_value_t));
    }

    return record;
}

void record_free(record_t *record) {
    assert(record != NULL);
    record->size = 0;
    record->num_key_values = 0;
    free(record);
}

buffer_t *bufferize_record(record_t *record) {
    assert(record != NULL);
    buffer_t *buffer = buffer_init(0);
    op_status st = buffer_write(buffer, &record->size, sizeof(uint32_t));
    if (st == ERROR) {
        buffer_free(buffer);
        return NULL;
    }
    st = buffer_write(buffer, &record->num_key_values, sizeof(uint32_t));
    if (st == ERROR) {
        buffer_free(buffer);
        return NULL;
    }
    for (size_t i = 0; i < record->num_key_values; i++) {
        key_value_t key_value = record->key_values[i];
        char *key = key_value.key;
        st = buffer_write(buffer, key, strlen(key) + 1);
        if (st == ERROR) {
            buffer_free(buffer);
            return NULL;
        }
        st = buffer_write(buffer, &key_value.value_type, sizeof(value_type_t));
        if (st == ERROR) {
            buffer_free(buffer);
            return NULL;
        }
        switch (key_value.value_type) {
            case INT_VALUE:
                st = buffer_write(buffer, &key_value.value.int_value, sizeof(int64_t));
                if (st == ERROR) {
                    buffer_free(buffer);
                    return NULL;
                }
                break;
            case DOUBLE_VALUE:
                st = buffer_write(buffer, &key_value.value.float_value, sizeof(double));
                if (st == ERROR) {
                    buffer_free(buffer);
                    return NULL;
                }
                break;
            case STRING_VALUE:
                st = buffer_write(buffer, key_value.value.string_value,
                                  strlen((const char *) &key_value.value) + 1);
                if (st == ERROR) {
                    buffer_free(buffer);
                    return NULL;
                }
                break;
            case BOOL_VALUE:
                st = buffer_write(buffer, &key_value.value.bool_value, sizeof(bool));
                if (st == ERROR) {
                    buffer_free(buffer);
                    return NULL;
                }
                break;
            case STRUCT_VALUE:
                st = buffer_write(buffer, key_value.value.key_value, sizeof(key_value_t));
                if (st == ERROR) {
                    buffer_free(buffer);
                    return NULL;
                }
                break;
        }

    }
    return buffer;
}

buffer_t *bufferize_page(page_t *page) {
    buffer_t *buffer = buffer_init(0);
    page_header_t *header = page->header;
    op_status st = buffer_write(buffer, &header->record_size, sizeof(uint32_t));
    if (st == ERROR) {
        buffer_free(buffer);
        return NULL;
    }
    st = buffer_write(buffer, &header->last_record_offset, sizeof(uint32_t));
    if (st == ERROR) {
        buffer_free(buffer);
        return NULL;
    }
    st = buffer_write(buffer, page->header->next, sizeof(uint32_t));
    if (st == ERROR) {
        buffer_free(buffer);
        return NULL;
    }
    st = buffer_write(buffer, page->header->prev, sizeof(uint32_t));
    if (st == ERROR) {
        buffer_free(buffer);
        return NULL;
    }
    uint32_t record_amount = (PAGE_SIZE - sizeof(page_header_t)) / page->header->record_size;

    for (uint32_t i = 0; i < record_amount; i++) {
        buffer_t *record_buffer = bufferize_record((record_t *)page->data[i]);
        st = buffer_write(buffer, record_buffer, record_buffer->size);
        if (st == ERROR) {
            buffer_free(buffer);
            return NULL;
        }
        buffer_free(record_buffer);
    }

    return buffer;
}

op_status write_record(allocator_t *allocator, record_t *record) {
    assert(allocator != NULL);
    assert(record != NULL);
    buffer_t *buffer = bufferize_record(record);
    if (buffer == NULL || buffer->size > allocator->page->header->record_size) {
        return ERROR;
    }
    uint64_t buffer_size = buffer->size;
    offset_t offset = allocator->page->header->last_record_offset;
    if (buffer_size > PAGE_SIZE - sizeof(page_header_t)) {
        while (buffer_size > 0) {
            uint64_t space_left = PAGE_SIZE - offset;
            uint64_t data_to_write;
            if (buffer_size <= space_left) {
                data_to_write = buffer_size;
            }
            else {
                data_to_write = space_left;
            }

            memcpy(allocator->page->data + offset, buffer->data +
                (buffer->size - buffer_size), data_to_write);

            allocator->page->header->last_record_offset += data_to_write;
            buffer_size -= data_to_write;

            if (buffer_size > 0) {
                buffer_t *buffer_page = bufferize_page(allocator->page);
                if (buffer_page == NULL) {
                    buffer_free(buffer);
                    return ERROR;
                }
                file_handler_save(buffer_page,
                                  allocator->file_handler);
                op_status st = create_page(allocator);
                if (st == ERROR) {
                    return ERROR;
                }
            }
        }
    } else {
        page_header_t *header = allocator->page->header;
        memcpy(allocator->page->data + offset, buffer->data, header->record_size);
        header->last_record_offset += header->record_size;
    }
    buffer_free(buffer);
    return SUCCESS;
}

record_t *read_record(allocator_t *allocator, size_t index) {
    assert(allocator != NULL);

    if (allocator->page == NULL) {
        return NULL;
    }

    page_t *page = allocator->page;
    page_header_t *header = page->header;

    offset_t offset = header->record_size * index;

    if (offset >= header->last_record_offset) {
        return NULL;
    }

    uint32_t size;
    memcpy(&size, page->data + offset, sizeof(uint32_t));

    if (size > header->record_size) {
        return NULL;
    }

    buffer_t *record_buffer = buffer_init(0);
    if (record_buffer == NULL) {
        return NULL;
    }

    op_status st = buffer_write(record_buffer, page->data + offset, size);
    if (st == ERROR) {
        buffer_free(record_buffer);
        return NULL;
    }
    offset = sizeof(uint32_t);
    uint32_t *num_key_values = (uint32_t *)buffer_read(record_buffer, offset);
    if (num_key_values == NULL) {
        buffer_free(record_buffer);
        return NULL;
    }
    offset += sizeof(uint32_t);
    char *key_val = buffer_read(record_buffer, offset);
    char *key = malloc(sizeof(strlen(key_val) + 1));
    strcpy(key, key_val);
    if (key == NULL) {
        buffer_free(record_buffer);
        return NULL;
    }
    offset += strlen(key) + 1;
    value_type_t *value_type = (value_type_t *)buffer_read(record_buffer, offset);
    if (value_type == NULL) {
        buffer_free(record_buffer);
        return NULL;
    }
    offset += sizeof(value_type_t);
    char *value = malloc(sizeof(union value_t));
    *value = *buffer_read(record_buffer, offset);
    if (value == NULL) {
        buffer_free(record_buffer);
        return NULL;
    }
    key_value_t *key_value = alloc_key_value(key, *value_type, value);
    record_t *record = create_record(key_value,1);

    buffer_free(record_buffer);

    return record;
}

