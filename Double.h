#ifndef DOUBLE_H
#define DOUBLE_H

#include "TypeInfo.h"

void doubleSum(const void* arg1, const void* arg2, void* result);
void doubleSubtract(const void* arg1, const void* arg2, void* result);
void doubleMultiply(const void* arg1, const void* arg2, void* result);
void doublePrint(const void* data);
void doubleScan(void* coeff);
TypeInfo* GetDoubleTypeInfo();

#endif // DOUBLE_H
