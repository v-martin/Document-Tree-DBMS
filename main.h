//
// Created by AboHolo on 26-Sep-23.
//

#include <bits/stdint-uintn.h>

#ifndef DOCUMENTTREE_MAIN_H
#define DOCUMENTTREE_MAIN_H

#endif //DOCUMENTTREE_MAIN_H

enum data_type {
    bool_t = 0,
    int_t,
    string_t,
    struct_t,
    float_t,
};

struct metadata {

};

struct db_header {
    uint64_t key;
    enum data_type* types;
};

struct database {
    char* name;
    struct db_header* header;
};


