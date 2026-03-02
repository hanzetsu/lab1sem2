#ifndef TYPEINFO_H
#define TYPEINFO_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef void (*BinaryOperator)(const void* arg1, const void* arg2, void* result);
typedef void (*UnaryOperator)(const void* lf, void* n, void* result); // надо уточнить n
typedef struct {
    uint64_t size;
    BinaryOperator add;
    UnaryOperator multiplyByInt;
    UnaryOperator multiplyByDouble;
    void (*print)(const void*);
} TypeInfo;

#endif 
