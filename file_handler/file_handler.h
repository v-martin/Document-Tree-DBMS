//
// Created by AboHolo on 25-Oct-23.
//
#ifndef DOCUMENTTREE_FILE_HANDLER_H
#define DOCUMENTTREE_FILE_HANDLER_H

#include <stdio.h>
#include "../buffer/buffer.h"

typedef struct file_handler_t {
    FILE *file;
} file_handler_t;



file_handler_t *file_handler_init(const char* filename);

op_status file_handler_free(file_handler_t *file_handler);

op_status file_handler_save(buffer_t *buffer, file_handler_t *file_handler);

buffer_t *file_handler_read(uint32_t size, file_handler_t *file_handler);

#endif //DOCUMENTTREE_FILE_HANDLER_H
