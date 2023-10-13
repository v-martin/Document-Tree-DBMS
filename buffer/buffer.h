//
// Created by AboHolo on 13-Oct-23.
//

#ifndef DOCUMENTTREE_BUFFER_H
#define DOCUMENTTREE_BUFFER_H

#include <stdint.h>

typedef struct {
    uint64_t size;
    char *data;
}buffer_t;

typedef enum {
    SUCCESS,
    ERROR,
} op_status;

buffer_t *buffer_init(uint64_t size);

void buffer_free(buffer_t *buffer);

op_status buffer_write(buffer_t *buffer, const void *data, uint64_t data_size);

void *buffer_read(const buffer_t *buffer, uint64_t offset);

#endif //DOCUMENTTREE_BUFFER_H
