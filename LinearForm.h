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


LinearForm* createLinearForm(TypeInfo* typeInfo, void* coeffs, uint32_t n, LinearFormErrors* operationResult);
void freeLinearForm(LinearForm* lf);
LinearFormErrors sumLinearForms(const LinearForm* a, const LinearForm* b, LinearForm* result);
LinearFormErrors subtractLinearForms(const LinearForm* a, const LinearForm* b, LinearForm* result);
LinearFormErrors multiplyScalarLinearForm(const LinearForm* lf, const void* scalar, LinearForm* result);
LinearFormErrors evaluateLinearForm(const LinearForm* lf, const void* args, void* result);
LinearFormErrors printLinearForm(const LinearForm* lf);

