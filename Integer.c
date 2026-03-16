#include "integer.h"
#include <stdio.h>
#include <stdlib.h>

static TypeInfo *INT_TYPE_INFO = NULL;

void intSum(const void *arg1, const void *arg2, void *result)
{
    *(int *)result = *(int *)arg1 + *(int *)arg2;
}

void intSubtract(const void *arg1, const void *arg2, void *result)
{
    *(int *)result = *(int *)arg1 - *(int *)arg2;
}

void intMultiply(const void *arg1, const void *arg2, void *result)
{
    *(int *)result = *(int *)arg1 * *(int *)arg2;
}

void intPrint(const void *data)
{
    printf("%d", *(const int *)data);
}

void intScan(void *coeff)
{
    int *val = (int *)coeff;
    char next;
    for (;;)
    {
        if (scanf("%d%c", val, &next) == 2 && (next == '\n' || next == ' '))
        {
            if (next == ' ')
            {
                fflush(stdin);
            }
            return;
        }
        else
        {
            puts("Ошибка: некорректный ввод. Ожидается целое число.");
            fflush(stdin);
        }
    }
}

TypeInfo *GetIntTypeInfo()
{
    if (INT_TYPE_INFO == NULL)
    {
        INT_TYPE_INFO = (TypeInfo *)malloc(sizeof(TypeInfo));
        INT_TYPE_INFO->size = sizeof(int);
        INT_TYPE_INFO->sum = intSum;
        INT_TYPE_INFO->subtract = intSubtract;
        INT_TYPE_INFO->multiply = intMultiply;
        INT_TYPE_INFO->print = intPrint;
        INT_TYPE_INFO->scan = intScan;
    }
    return INT_TYPE_INFO;
}