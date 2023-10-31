#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../file_handler/file_handler.h"
#include "../buffer/buffer.h"

int main() {
    const char* filename = "data.txt";

    file_handler_t *file_handler = file_handler_init(filename);
    if (file_handler == NULL) {
        fprintf(stderr, "Failed to initialize the file handler.\n");
        return 1;
    }

    const char* sample_data = "Hello, World!";
    buffer_t *buffer = buffer_init(0);
    if (buffer == NULL) {
        fprintf(stderr, "Failed to initialize the buffer.\n");
        file_handler_free(file_handler);
        return 1;
    }
    op_status st = buffer_write(buffer, sample_data, strlen(sample_data) + 1);
    if (st == ERROR) {
        return 1;
    }

    op_status save_status = file_handler_save(buffer, file_handler);
    if (save_status != SUCCESS) {
        fprintf(stderr, "Failed to save the buffer to the file.\n");
        buffer_free(buffer);
        file_handler_free(file_handler);
        return 1;
    }

    buffer_free(buffer);

    buffer_t *read_buffer = file_handler_read(strlen(sample_data) + 1, file_handler);
    if (read_buffer == NULL) {
        fprintf(stderr, "Failed to read from the file.\n");
        file_handler_free(file_handler);
        return 1;
    }
    char *output_string = (char *)buffer_read(read_buffer, 0);
    if (output_string != NULL && strcmp(output_string, sample_data) == 0) {
        printf("File content matches the sample data.\n");
    } else {
        printf("File content does not match the sample data.\n");
        printf("%s\n", output_string);
    }

    buffer_free(read_buffer);
    file_handler_free(file_handler);

    return 0;
}
