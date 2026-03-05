#pragma once

#include "TypeInfo.h"
#include "Integer.h"
#include "Double.h"
#include "LinearFormErrors.h"
#define MAX(a,b) ((a)>(b) ? (a) : (b))
#define MIN(a,b) ((a)<(b) ? (a) : (b))
typedef struct {
    void* coeffs;
    uint32_t n;
    TypeInfo* typeInfo;
} LinearForm;


LinearForm* createLinearForm(TypeInfo* typeInfo, void* coeffs, uint64_t n, LinearFormErrors* operationResult);
void freeLinearForm(LinearForm* lf);
LinearFormErrors addLinearForms(const LinearForm* a, const LinearForm* b, LinearForm* result);
LinearFormErrors multiplyScalarLinearForm(const LinearForm* lf, const void* scalar, LinearForm* result);
LinearFormErrors printLinearForm(const LinearForm* lf);

