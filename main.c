#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

#define PAGE_SIZE 4096

void create_db(const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char page[PAGE_SIZE];
    memset(page, 0, PAGE_SIZE);

    size_t bytes_written = fwrite(page, 1, PAGE_SIZE, file);
    if (bytes_written != PAGE_SIZE) {
        perror("Error writing to file");
    }

    fclose(file);
}

int main() {
    const char *filename = "data.txt";
    create_db(filename);
    printf("Page of size %d bytes written to %s\n", PAGE_SIZE, filename);
    return 0;
}
