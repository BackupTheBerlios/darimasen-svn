/* Darimasen - supplimental.h - Copyright (C) 2004 - 2005 Sudrien, GPL */

#ifndef SUPPLIMENTAL_H
#define SUPPLIMENTAL_H

/**********************/

#include "main.h"
#include <iostream>

/**********************/

Glib::ustring int2ustr(int x);

/**********************

#define log_xor || log_xor_helper() ||

struct log_xor_helper {
    bool value;
};

template<typename LEFT>
log_xor_helper &operator ||(const LEFT &left, log_xor_helper &xor) {
    xor.value = (bool)left;
    return xor;
}

template<typename RIGHT>
bool operator ||(const log_xor_helper &xor, const RIGHT &right) {
    return xor.value ^ (bool)right;
}

/**********************/

#endif //SUPPLIMENTAL_H

