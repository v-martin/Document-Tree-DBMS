//
// Created by AboHolo on 13-Oct-23.
//

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "buffer.h"

buffer_t *buffer_init(uint64_t size) {
    buffer_t  *buffer = malloc(sizeof(buffer_t));
    if (buffer == NULL) {
        return NULL;
    }
    buffer->size = size;
    if (size > 0) {
        buffer->data = malloc(size);
        if (buffer->data == NULL) {
            free(buffer);
            return NULL;
        }
    }
    else {
        buffer->data = NULL;
    }
    return buffer;
}

void buffer_free(buffer_t *buffer) {
    assert(buffer != NULL);
    free(buffer->data);
    buffer->data = NULL;
    buffer->size = 0;
    free(buffer);
}

op_status buffer_write(buffer_t *buffer, const void *data, uint64_t data_size) {
    assert(buffer != NULL);
    uint64_t new_size = buffer->size + data_size;
    char *new_data = (char *)realloc(buffer->data, new_size);
    if (new_data == NULL) {
        return ERROR;
    }
    buffer->data = new_data;
    buffer->size = new_size;
    memcpy(buffer->data + buffer->size - data_size, data, data_size);
    return SUCCESS;
}

char *buffer_read(const buffer_t *buffer, uint64_t offset) {
    assert(buffer != NULL);
    if (offset >= buffer->size) {
        return NULL;
    }
    char *string = buffer->data + offset;

    return string;
}

