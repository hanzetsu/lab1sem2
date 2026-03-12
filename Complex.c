#include "Complex.h"
#include <stdio.h>
#include <stdlib.h>

static TypeInfo* COMPLEX_TYPE_INFO = NULL;

void complexSum(const void* arg1, const void* arg2, void* result) {
    const Complex* a = (const Complex*)arg1;
    const Complex* b = (const Complex*)arg2;
    Complex* r = (Complex*)result;
    r->real = a->real + b->real;
    r->imag = a->imag + b->imag;
}

void complexSubtract(const void* arg1, const void* arg2, void* result) {
    const Complex* a = (const Complex*)arg1;
    const Complex* b = (const Complex*)arg2;
    Complex* r = (Complex*)result;
    r->real = a->real - b->real;
    r->imag = a->imag - b->imag;
}

void complexMultiply(const void* arg1, const void* arg2, void* result) {
    const Complex* a = (const Complex*)arg1;
    const Complex* b = (const Complex*)arg2;
    Complex* r = (Complex*)result;
    r->real = a->real * b->real - a->imag * b->imag;
    r->imag = a->real * b->imag + a->imag * b->real;
}

void complexPrint(const void* data) {
    const Complex* c = (const Complex*)data;
    if (c->imag >= 0)
        printf("%lf + %lfi", c->real, c->imag);
    else
        printf("%lf - %lfi", c->real, -c->imag);
}

void complexScan(void* data) {
    Complex* c = (Complex*)data;
    char next;
    for (;;)
    {
        if (scanf("%lf%lf%c", &c->real, &c->imag, &next) == 3 && (next == '\n' || next == ' '))
        {
            if (next == ' ')
            {
                int ch;
                while ((ch = getchar()) != '\n' && ch != EOF);
            }
            return;
        }
        else
        {
            puts("Ошибка: некорректный ввод. Ожидаются два вещественных числа.");
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
        }
    }
}

TypeInfo* GetComplexTypeInfo() {
    if (COMPLEX_TYPE_INFO == NULL) {
        COMPLEX_TYPE_INFO = (TypeInfo*)malloc(sizeof(TypeInfo));
        if (COMPLEX_TYPE_INFO) {
            COMPLEX_TYPE_INFO->size = sizeof(Complex);
            COMPLEX_TYPE_INFO->sum = complexSum;
            COMPLEX_TYPE_INFO->subtract = complexSubtract;
            COMPLEX_TYPE_INFO->multiply = complexMultiply;
            COMPLEX_TYPE_INFO->print = complexPrint;
            COMPLEX_TYPE_INFO->scan = complexScan;
        }
    }
    return COMPLEX_TYPE_INFO;
}