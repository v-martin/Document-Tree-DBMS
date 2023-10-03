#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "main.h"

#define PAGE_SIZE 4096


struct allocator_t* allocator_init(const char* filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }
    struct allocator_t* allocator = malloc(sizeof(struct allocator_t));
    if (allocator == NULL) {
        perror("");
        return NULL;
    }
    allocator->file = file;
    return allocator;
}

operation_status allocator_free(struct allocator_t* allocator) {
    assert(allocator != NULL);
    FILE* file = allocator->file;
    if (file == NULL) {
        return ERROR;
    }
    int status = fclose(file);
    if (status == 0) {
        free(allocator);
        return SUCCESS;
    }
    return ERROR;
}

struct database_t* db_init(const char* filename) {
    assert(filename != NULL);
    struct database_t* database = malloc(sizeof(struct database_t));
    if (database == NULL) {
        return NULL;
    }
    database->allocator = allocator_init(filename);
    if (database->allocator == NULL){
        free(database->allocator);
        free(database);
        return NULL;
    }
    return database;
}

operation_status db_free(struct database_t* database) {
    assert(database != NULL);
    operation_status status = allocator_free(database->allocator);
    if (status == ERROR){
        free(database);
        return ERROR;
    }
    free(database);
    return SUCCESS;
}

struct record write_record(const struct database_t database) {

}

int main() {
    const char *filename = "data.txt";
    struct database_t* database = db_init(filename);
    operation_status deletion_status = db_free(database);
    if (deletion_status == ERROR) {
        perror("Big L");
    }
    printf("Big W");
    return 0;
}
