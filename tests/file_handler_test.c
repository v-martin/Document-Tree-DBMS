//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include "../file_handler/file_handler.h"
//#include "../buffer/buffer.h"
//
//int main() {
//    const char* filename = "data.txt";
//
//    file_handler_t *file_handler = file_handler_init(filename);
//    if (file_handler == NULL) {
//        fprintf(stderr, "Failed to initialize the file handler.\n");
//        return 1;
//    }
//
//    const char* sample_string = "Hello, World!";
//    buffer_t *buffer = buffer_init(0);
//    if (buffer == NULL) {
//        fprintf(stderr, "Failed to initialize the buffer.\n");
//        file_handler_free(file_handler);
//        return 1;
//    }
//    op_status st = buffer_write(buffer, sample_string, strlen(sample_string) + 1);
//    if (st == ERROR) {
//        return 1;
//    }
//    int64_t sample_int = 42;
//    st = buffer_write(buffer, &sample_int, sizeof(int64_t));
//    if (st == ERROR) {
//        return 1;
//    }
//
//
//    op_status save_status = file_handler_save(buffer, file_handler);
//    if (save_status != SUCCESS) {
//        fprintf(stderr, "Failed to save the buffer to the file.\n");
//        buffer_free(buffer);
//        file_handler_free(file_handler);
//        return 1;
//    }
//
//    buffer_free(buffer);
//
//    buffer_t *read_buffer = file_handler_read(strlen(sample_string) + 1 + sizeof(int64_t), file_handler);
//    if (read_buffer == NULL) {
//        fprintf(stderr, "Failed to read from the file.\n");
//        file_handler_free(file_handler);
//        return 1;
//    }
//    char *output_string = (char *)buffer_read(read_buffer, 0);
//    int64_t *output_int = (int64_t *)buffer_read(read_buffer, strlen(output_string) + 1);
//    if (output_string != NULL && strcmp(output_string, sample_string) == 0 && *output_int == sample_int) {
//        printf("File content matches the sample data.\n");
//    } else {
//        printf("File content does not match the sample data.\n");
//        printf("%s\n", output_string);
//    }
//
//    buffer_free(read_buffer);
//    file_handler_free(file_handler);
//
//    return 0;
//}
