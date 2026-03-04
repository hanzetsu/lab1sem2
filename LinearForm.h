#pragma once

#include "TypeInfo.h"
#include "Integer.h"
#include "Double.h"
#include "LinearFormErrors.h"
#define MAX(a,b) ((a)>(b) ? (a) : (b))
#define MIN(a,b) ((a)<(b) ? (a) : (b))
typedef struct {
    void* coeffs;
    uint64_t n;
    TypeInfo* typeInfo;
} LinearForm;


LinearForm* createLinearForm(TypeInfo* typeInfo, void* coeffs, uint64_t n, LinearFormErrors* operationResult);
void freeLinearForm(LinearForm* lf);
LinearFormErrors addLinearForms(const LinearForm* a, const LinearForm* b, LinearForm* result);
LinearFormErrors multiplyByIntLinearForm(const LinearForm* lf, int x, LinearForm* result);
LinearFormErrors multiplyByDoubleLinearForm(const LinearForm* lf, double x, LinearForm* result);
LinearFormErrors printLinearForm(const LinearForm* lf);

