#include <stdio.h>
#include <stdlib.h>
#include "allocator/allocator.h"


int main() {
    const char *filename = "data.txt";
    database_t *database = db_init(filename);
    allocator_t *allocator = database->allocator;
    int num = 3;
    for (size_t i = 0; i < num; i++) {
        record_t record_in = {.key = i, .value = i * 2};
        if (write_record(allocator, &record_in) == ERROR) {
            printf("Error while writing %zu\n", i);
        }
        else printf("Writing complete %ld %ld\n", record_in.key, record_in.value);
    }
    for (size_t i = 0; i < num; i++) {
        record_t *record_out = read_record(allocator, i);
        if (record_out == NULL) {
            printf("Error reading %zu\n", i);
        } else printf("Reading complete %ld %ld\n", record_out->key, record_out->value);
    }

    if (db_free(database) == ERROR) {
        printf("Error deallocating");
    }
    else printf("Deallocating complete");

    return 0;
}
