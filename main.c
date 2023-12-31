#include <stdio.h>
#include <stdlib.h>
#include "allocator/allocator.h"


int main() {
    const char *filename = "data.txt";
    database_t *database = db_init(filename);
    allocator_t *allocator = database->allocator;
    int num = 1;
    for (size_t i = 0; i < num; i++) {
        key_value_t key_value = {.key = "rcrd", .value_type = INT_VALUE, .value = (int64_t) i};
        record_t *record_in = create_record(&key_value, 1);
        if (write_record(allocator, record_in) == ERROR) {
            printf("Error while writing %zu\n", i);
        }
        else {
            printf("Writing complete %s\n", record_in->key_values->key);
            record_free(record_in);
        }

    }
    for (size_t i = 0; i < num; i++) {
        record_t *record_out = read_record(allocator, i);
        if (record_out == NULL) {
            printf("Error reading %zu\n", i);
        } else printf("Reading complete %s\n", record_out->key_values->key);
    }

    if (db_free(database) == ERROR) {
        printf("Error deallocating");
    }
    else printf("Deallocating complete");

    return 0;
}
