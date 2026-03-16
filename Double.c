#include "double.h"
#include <stdio.h>
#include <stdlib.h>

static TypeInfo* DOUBLE_TYPE_INFO = NULL;

void doubleSum(const void* arg1, const void* arg2, void* result) {
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
    double *val = (double *)coeff;
    char next;
    for (;;)
    {
        if (scanf("%lf%c", val, &next) == 2 && (next == '\n' || next == ' '))
        {
            if (next == ' ')
            {
                fflush(stdin);
            }
            return;
        }
        else
        {
            puts("Ошибка: некорректный ввод. Ожидается вещественное число.");
            fflush(stdin);
        }
    }
}

TypeInfo* GetDoubleTypeInfo() {
    if (DOUBLE_TYPE_INFO == NULL) {
        DOUBLE_TYPE_INFO = (TypeInfo*)malloc(sizeof(TypeInfo));
        DOUBLE_TYPE_INFO->size = sizeof(double);
        DOUBLE_TYPE_INFO->sum = doubleSum;
        DOUBLE_TYPE_INFO->subtract = doubleSubtract;
        DOUBLE_TYPE_INFO->multiply = doubleMultiply;
        DOUBLE_TYPE_INFO->print = doublePrint;
        DOUBLE_TYPE_INFO->scan = doubleScan;
    }
    return DOUBLE_TYPE_INFO;
}