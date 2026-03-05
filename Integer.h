#ifndef INTEGER_H
#define INTEGER_H

#include "TypeInfo.h"

void intAdd(const void* arg1, const void* arg2, void* result);
void intSubtract(const void* arg1, const void* arg2, void* result);
void intMultiply(const void* arg1, const void* arg2, void* result);
void intPrint(const void* data);
void intScan(void* data);
TypeInfo* GetIntTypeInfo();

#endif