cmake_minimum_required(VERSION 3.16)
project(DocumentTree C)

set(CMAKE_C_STANDARD 11)

add_executable(DocumentTree main.c allocator/allocator.h allocator/allocator.h allocator/allocator.c allocator/allocator.c pool/pool.c buffer/buffer.c buffer/buffer.c buffer/buffer.h buffer/buffer.h pool/pool.h pool/pool.h tests/buffer_test.c tests/buffer_test.c file_handler/file_handler.c file_handler/file_handler.h file_handler/file_handler.h tests/file_handler_test.c)
