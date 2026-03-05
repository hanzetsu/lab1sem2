#include "LinearForm.h"
#include "TypeInfo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

LinearForm *createLinearForm(TypeInfo *typeInfo, const void *coeffs, uint32_t n, LinearFormErrors *operationResult)
{
    LinearForm *lf = (LinearForm *)malloc(sizeof(*lf));
    if (lf == NULL)
    {
        *operationResult = MEMORY_ALLOCATION_FAILED;
        return NULL;
    }

    lf->typeInfo = typeInfo;
    lf->n = n;
    lf->coeffs = malloc(typeInfo->size * (lf->n + 1));
    if (lf->coeffs == NULL)
    {
        free(lf);
        *operationResult = MEMORY_ALLOCATION_FAILED;
        return NULL;
    }
    if (coeffs != NULL)
    {
        memcpy(lf->coeffs, coeffs, typeInfo->size * (n + 1));
    }
    else
    {
        memset(lf->coeffs, 0, typeInfo->size * (n + 1));
    }
    *operationResult = LINEARFORM_OPERATION_OK;
    return lf;
}

void freeLinearForm(LinearForm *lf) {
    if (lf) {
        if (lf->coeffs) free(lf->coeffs);
        free(lf);
    }
}

LinearFormErrors addLinearForms(const LinearForm *a, const LinearForm *b, LinearForm *result)
{
    if (a == NULL || b == NULL || result == NULL)
        return LINEARFORM_NOT_DEFINED;
    if (a->typeInfo != b->typeInfo || a->typeInfo != result->typeInfo)
        return INCOMPATIBLE_LINEARFORM_TYPES;
    uint32_t max_n = MAX(a->n, b->n);
    if (result->n != max_n)
        return DIMENSION_MISMATCH;
    if (a->typeInfo->add == NULL)
        return OPERATION_NOT_DEFINED;

    void* zero = calloc(1, a->typeInfo->size);
    if (zero == NULL)
        return MEMORY_ALLOCATION_FAILED;

    for (uint32_t i = 0; i <= max_n; i++)
    {
        if (a->n >= i && b->n >= i)
            a->typeInfo->add((char *)a->coeffs + i * a->typeInfo->size,
                             (char *)b->coeffs + i * b->typeInfo->size,
                             (char *)result->coeffs + i * result->typeInfo->size);
        else if (a->n < i)
            a->typeInfo->add(zero,
                             (char *)b->coeffs + i * b->typeInfo->size,
                             (char *)result->coeffs + i * result->typeInfo->size);
        else 
            a->typeInfo->add((char *)a->coeffs + i * a->typeInfo->size,
                             zero,
                             (char *)result->coeffs + i * result->typeInfo->size);
    }
    free(zero);
    return LINEARFORM_OPERATION_OK;
}

LinearFormErrors subtractLinearForms(const LinearForm* a, const LinearForm* b, LinearForm* result) {
    if (a == NULL || b == NULL || result == NULL)
        return LINEARFORM_NOT_DEFINED;
    if (a->typeInfo != b->typeInfo || a->typeInfo != result->typeInfo)
        return INCOMPATIBLE_LINEARFORM_TYPES;
    uint32_t max_n = MAX(a->n, b->n);
    if (result->n != max_n)
        return DIMENSION_MISMATCH;
    if (a->typeInfo->subtract == NULL)
        return OPERATION_NOT_DEFINED;

    void* zero = calloc(1, a->typeInfo->size);
    if (zero == NULL)
        return MEMORY_ALLOCATION_FAILED;

    for (uint32_t i = 0; i <= max_n; i++)
    {
        if (a->n >= i && b->n >= i)
            a->typeInfo->subtract((char *)a->coeffs + i * a->typeInfo->size,
                                  (char *)b->coeffs + i * b->typeInfo->size,
                                  (char *)result->coeffs + i * result->typeInfo->size);
        else if (a->n < i)
            a->typeInfo->subtract(zero,
                                  (char *)b->coeffs + i * b->typeInfo->size,
                                  (char *)result->coeffs + i * result->typeInfo->size);
        else
            a->typeInfo->subtract((char *)a->coeffs + i * a->typeInfo->size,
                                  zero,
                                  (char *)result->coeffs + i * result->typeInfo->size);
    }
    free(zero);
    return LINEARFORM_OPERATION_OK;
}

LinearFormErrors multiplyScalarLinearForm(const LinearForm* lf, const void* scalar, LinearForm* result)
{
    if (lf == NULL || scalar == NULL || result == NULL)
        return LINEARFORM_NOT_DEFINED;
    if (lf->typeInfo != result->typeInfo)
        return INCOMPATIBLE_LINEARFORM_TYPES;
    if (result->n != lf->n)
        return DIMENSION_MISMATCH;
    if (lf->typeInfo->multiply == NULL)
        return OPERATION_NOT_DEFINED;

    for (uint32_t i = 0; i <= lf->n; i++) {
        const void* coeff_i = (const char *)lf->coeffs + i * lf->typeInfo->size;
        void* res_i = (char*)result->coeffs + i * result->typeInfo->size;
        lf->typeInfo->multiply(coeff_i, scalar, res_i);
    }
    return LINEARFORM_OPERATION_OK;
}

LinearFormErrors evaluateLinearForm(const LinearForm* lf, const void* args, void* result)
{
    if (lf == NULL || args == NULL || result == NULL)
        return LINEARFORM_NOT_DEFINED;
    if (lf->typeInfo->add == NULL || lf->typeInfo->multiply == NULL)
        return OPERATION_NOT_DEFINED;

    void* temp = malloc(lf->typeInfo->size);
    if (temp == NULL)
        return MEMORY_ALLOCATION_FAILED;

    memcpy(result, lf->coeffs, lf->typeInfo->size);

    for (uint32_t i = 1; i <= lf->n; i++) {
        const void* coeff_i = (const char*)lf->coeffs + i * lf->typeInfo->size;
        const void* arg_i = (const char*)args + (i-1) * lf->typeInfo->size;

        lf->typeInfo->multiply(coeff_i, arg_i, temp);
        lf->typeInfo->add(result, temp, result);
    }

    free(temp);
    return LINEARFORM_OPERATION_OK;
}

LinearFormErrors printLinearForm(const LinearForm *lf)
{
    if (lf == NULL)
        return LINEARFORM_NOT_DEFINED;
    if (lf->typeInfo->print == NULL)
        return OPERATION_NOT_DEFINED;

    lf->typeInfo->print(lf->coeffs);
    for (uint32_t i = 1; i <= lf->n; i++) {
        printf(" + ");
        const void *coeff = (const char*)lf->coeffs + i * lf->typeInfo->size;
        lf->typeInfo->print(coeff);
        printf("*x%u", i);
    }
    putchar('\n');
    return LINEARFORM_OPERATION_OK;
}