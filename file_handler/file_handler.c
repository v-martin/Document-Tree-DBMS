//
// Created by AboHolo on 25-Oct-23.
//

#include <malloc.h>
#include <assert.h>
#include <stdint.h>
#include "file_handler.h"

file_handler_t *file_handler_init(const char* filename) {
    assert(filename != NULL);
    file_handler_t *file_handler = malloc(sizeof(file_handler_t));
    if (file_handler == NULL) {
        return NULL;
    }
    FILE *file = fopen(filename, "r+");
    if (file == NULL) {
        free(file_handler);
        return NULL;
    }
    file_handler->file = file;
    return file_handler;
}

op_status file_handler_free(file_handler_t *file_handler) {
    assert(file_handler != NULL);
    int st = fclose(file_handler->file);
    if (st != 0) {
        return ERROR;
    }
    free(file_handler);
    return SUCCESS;
}

op_status file_handler_save(buffer_t *buffer, file_handler_t *file_handler) {
    assert(buffer != NULL);
    int st_fseek = fseek(file_handler->file, 0, SEEK_END);
    if (st_fseek != 0) {
        return ERROR;
    }
    size_t items_created = fwrite(buffer->data, buffer->size, 1, file_handler->file);
    if (items_created != 1) {
        return ERROR;
    }
    return SUCCESS;
}

buffer_t *file_handler_read(uint32_t size, file_handler_t *file_handler) {
    assert(file_handler != NULL);
    buffer_t *buffer = buffer_init(size);
    if (buffer == NULL) {
        return NULL;
    }
    int st_fseek = fseek(file_handler->file, 0, SEEK_END);
    if (st_fseek != 0) {
        buffer_free(buffer);
        return NULL;
    }
    long file_size = ftell(file_handler->file);
    if (file_size < size) {
        buffer_free(buffer);
        return NULL;
    }
    st_fseek = fseek(file_handler->file, 0, SEEK_SET);
    if (st_fseek != 0) {
        buffer_free(buffer);
        return NULL;
    }
    size_t items_read = fread(buffer->data, 1, size, file_handler->file);
    if (items_read != size) {
        buffer_free(buffer);
        return NULL;
    }
    return buffer;
}

