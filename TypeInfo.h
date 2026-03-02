#ifndef TYPEINFO_H
#define TYPEINFO_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef void (*BinaryOperator)(const void* arg1, const void* arg2, void* result);
typedef struct {
    uint64_t size;
    BinaryOperator add;
    BinaryOperator subtract; 
    BinaryOperator multiply;
    void (*print)(const void*);
} TypeInfo;
#endif 
