#ifndef COMPLEX_H
#define COMPLEX_H

#include "TypeInfo.h"

typedef struct {
    double real;
    double imag;
} Complex;

void complexAdd(const void* arg1, const void* arg2, void* result);
void complexSubtract(const void* arg1, const void* arg2, void* result);
void complexMultiply(const void* arg1, const void* arg2, void* result);
void complexPrint(const void* data);
void complexScan(void* data);
TypeInfo* GetComplexTypeInfo();

#endif