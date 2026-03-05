#include "double.h"

static TypeInfo* DOUBLE_TYPE_INFO = NULL;


void doubleAdd(const void* arg1, const void* arg2, void* result) {
    *(double*)result = *(double*)arg1 + *(double*)arg2;
}
void doubleSubtract(const void* arg1, const void* arg2, void* result) {
    *(double*)result = *(double*)arg1 - *(double*)arg2;
}

void doubleMultiply(const void* arg1, const void* arg2, void* result) {
    *(double*)result = *(double*)arg1 * *(double*)arg2;
}

void doublePrint(const void* data) {
    printf("%lf", *(const double*)data);
}

void doubleScan(void* coeff) {
    scanf("%lf", (double*)coeff);
}

TypeInfo* GetDoubleTypeInfo() {
    if (DOUBLE_TYPE_INFO == NULL) {
        DOUBLE_TYPE_INFO = (TypeInfo*)malloc(sizeof(TypeInfo));
        DOUBLE_TYPE_INFO->size = sizeof(double);
        DOUBLE_TYPE_INFO->add = doubleAdd;
        DOUBLE_TYPE_INFO->subtract = doubleSubtract;
        DOUBLE_TYPE_INFO->multiply = doubleMultiply;
        DOUBLE_TYPE_INFO->print = doublePrint;
        DOUBLE_TYPE_INFO->scan = doubleScan;
    }
    return DOUBLE_TYPE_INFO;
}