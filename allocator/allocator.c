//
// Created by AboHolo on 11-Oct-23.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "allocator.h"

#define PAGE_SIZE 64


op_status save_page(struct page_t *pPage, FILE *pFile);

allocator_t *allocator_init(const char* filename) {
    FILE *file = fopen(filename, "r+");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }
    struct allocator_t* allocator = malloc(sizeof(allocator_t));
    if (allocator == NULL) {
        perror("");
        return NULL;
    }
    allocator->file = file;
    op_status st = create_page(allocator);
    if (st == ERROR) {
        free(allocator);
        return NULL;
    }
    return allocator;
}

op_status allocator_free(allocator_t *allocator) {
    assert(allocator != NULL);
    assert(allocator->file != NULL);
    free(allocator->page);
    FILE *file = allocator->file;
    if (file == NULL) {
        return ERROR;
    }
    int status = fclose(file);
    if (status == 0) {
        free(allocator);
        return SUCCESS;
    }
    else {
        free(allocator);
        return ERROR;
    }
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

op_status create_page_header(page_t *page) {
    page_header_t *header = malloc(sizeof(page_header_t));
    if (header == NULL) {
        free(header);
        return ERROR;
    }
    header->last_record_offset = sizeof(page_header_t);
    page->header = header;
    return SUCCESS;
}

op_status create_page(allocator_t *allocator) {
    page_t *page = malloc(sizeof(page_t));
    op_status st_header = create_page_header(page);
    if (st_header == ERROR) {
        free(page);
        return ERROR;
    }
    page->first_record = (offset_t) NULL;
    page->last_record = (offset_t) NULL;

    if (allocator->page == NULL) {
        page->prev = NULL;
        page->next = NULL;
    }
    else {
        page->prev = allocator->page;
        allocator->page->next = page;
    }
    allocator->page = page;
    return SUCCESS;
}

op_status save_page(struct page_t *page, FILE *file) {
    int st_fseek = fseek(file, 0, SEEK_END);
    if (st_fseek != 0) {
        return ERROR;
    }
    size_t items_created = fwrite(page, PAGE_SIZE, 1, file);
    if (items_created != 1) {
        free(page);
        return ERROR;
    }
    return SUCCESS;
}


offset_t append_to_page(allocator_t *allocator, const record_t *record) {
    assert(allocator != NULL);
    assert(record != NULL);
    page_t *page = allocator->page;
    offset_t offset = page->header->last_record_offset;
    if (offset < PAGE_SIZE) {
        record_t *record_ptr = (record_t *) page + offset;
        if (record_ptr == NULL) {
            return (offset_t) NULL;
        }
        *record_ptr = *record;
        offset += sizeof(record_t);
        if (offset <= PAGE_SIZE) {
            page->header->last_record_offset = offset;
        }
        else {
            page->header->last_record_offset = PAGE_SIZE;
            offset = PAGE_SIZE;
        }

        return offset;
    }
    page_t *page_old = page;
    op_status st_create = create_page(allocator);
    op_status st_save = save_page(allocator->page, allocator->file);
    if (st_save == ERROR || st_create == ERROR) {
            return ERROR;
    }
    free(allocator->page);
    return 0;
}

op_status write_record(allocator_t *allocator, record_t *record) {
    assert(allocator != NULL);
    assert(record != NULL);
    page_t *page = allocator->page;
    offset_t offset = append_to_page(allocator, record);
    if (offset == 0)
        return ERROR;
    if (page->first_record == 0 && page->last_record == 0) {
        page->first_record = offset;
    }
    else {
        page->last_record = offset;
    }
    return SUCCESS;
}

//    FILE* file = allocator->file;
//    if (ftell(file) != EOF) {
//        fseek(file, 0, SEEK_END);
//    }
//    struct record_t* cur_record = malloc(sizeof(*record));
//    cur_record->key = record->key;
//    cur_record->value = record->value;
//    size_t items_written = fwrite(cur_record, sizeof(*record), 1, file);
//    if (items_written != 1) {
//        free(cur_record);
//        return ERROR;
//    }
//    free(cur_record);
//    fseek(file, 0, SEEK_END);
//    return SUCCESS;


record_t *read_record(const allocator_t *allocator, size_t index) {
    assert(allocator != NULL);
    page_t *page = allocator->page;
    offset_t offset = (offset_t) sizeof(page_header_t) + (sizeof(record_t) * index);
    if (offset < PAGE_SIZE) {
        record_t *record = (record_t*) page + offset;
        return record;
    }
    else
        return NULL;
}
