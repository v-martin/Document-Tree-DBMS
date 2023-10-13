//
// Created by AboHolo on 13-Oct-23.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../buffer/buffer.h"


int main() {
    buffer_t *buffer = buffer_init(0);
    int64_t example_int = 42;
    double example_double = 3.14159;
    const char *example_string = "Hello, world!";
    bool example_bool = true;

    buffer_write(buffer, &example_int, sizeof(int64_t));
    buffer_write(buffer, &example_double, sizeof(double));
    buffer_write(buffer, example_string, strlen(example_string) + 1);
    buffer_write(buffer, &example_bool, sizeof(bool));

    int64_t *read_int = (int64_t*)buffer_read(buffer, 0);
    double *read_double = (double*)buffer_read(buffer, sizeof(double));
    char *read_string_value = (char*)buffer_read(buffer, sizeof(int64_t) + sizeof(double));
    bool *read_bool_value = (bool*)buffer_read(buffer, sizeof(int64_t) + sizeof(double) + 1);

    printf("Buffer size: %lu\n", buffer->size);
    printf("Buffer data: ");
    for (uint64_t i = 0; i < buffer->size; i++) {
        printf("%02X ", (unsigned char)buffer->data[i]);
    }
    printf("\n");

    printf("Read Int: %ld\n", *read_int);
    printf("Read Double: %lf\n", *read_double);
    printf("Read String: %s\n", read_string_value);
    printf("Read Bool: %s\n", *read_bool_value ? "true" : "false");

    buffer_free(buffer);

    return 0;
}

