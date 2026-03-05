#include "LinearForm.h"
#include "TypeInfo.h"

LinearForm *createLinearForm(TypeInfo *typeInfo, const void *coeffs, uint64_t n, LinearFormErrors *operationResult)
{
    LinearForm *lf = (LinearForm *)malloc(sizeof(*lf));
    if (lf == NULL)
    {
        *operationResult = MEMORY_ALLOCATION_FAILED;
        return NULL;
    }

    lf->typeInfo = typeInfo;
    lf->n = n;
    lf->coeffs = malloc(typeInfo->size * lf->(n + 1));
    if (lf->coeffs == NULL)
    {
        freeLinearForm(lf);
        operationResult = MEMORY_ALLOCATION_FAILED;
        return NULL;
    }
    if (coeffs != NULL)
    {
        memcpy(lf->coeffs, coeffs, typeInfo->size * (n + 1));
    }
    else
        memset(lf->coeffs, 0, typeInfo->size * (n + 1));
    *operationResult = LINEARFORM_OPERATION_OK;
    return lf;
}

void freeLinearForm(LinearForm *lf)
{
    free(lf->coeffs);
    free(lf);
}

LinearFormErrors addLinearForms(const LinearForm *a, const LinearForm *b, LinearForm *result)
{
    if (a == NULL || b == NULL || result == NULL)
        return LINEARFORM_NOT_DEFINED;
    if (a->typeInfo != b->typeInfo || a->typeInfo != result->typeInfo)
        return INCOMPATIBLE_LINEARFORM_TYPES;
    uint64_t max_n = MAX(a->n, b->n);
    if (result->n < max_n)
        return DIMENSION_MISMATCH;
    if (a->typeInfo->add == NULL)
        return OPERATION_NOT_DEFINED;
    void* zero = calloc(1,a->typeInfo->size);
    for (uint64_t i = 0; i < max_n + 1; i++)
    {
        if (a->n >= i && b->n >= i)
            a->typeInfo->add((char *)a->coeffs + (i * a->typeInfo->size), (char *)b->coeffs + (i * b->typeInfo->size), (char *)result->coeffs + (i * result->typeInfo->size));
        else if (a->n < i)
            a->typeInfo->add(zero, (char *)b->coeffs + (i * b->typeInfo->size), (char *)result->coeffs + (i * result->typeInfo->size));
        else if (b->n < i)
            a->typeInfo->add((char *)a->coeffs + (i * a->typeInfo->size), zero, (char *)result->coeffs + (i * result->typeInfo->size));
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
    for (uint64_t i = 0; i <= lf->n; i++) {
        const void* coeff_i = (const char *)lf->coeffs + (i*lf->typeInfo->size);
        void* res_i = (char*)result->coeffs + (i * result->typeInfo->size);
        lf->typeInfo->multiply(coeff_i, scalar, res_i);
    }
    return LINEARFORM_OPERATION_OK;
}

VectorErrors printVector(const Vector *vector)
{
    if (vector == NULL)
        return VECTOR_NOT_DEFINED;
    if (vector->typeInfo->print == NULL)
        return OPERATION_NOT_DEFINED;

    printf("Vector: (");
    vector->typeInfo->print(vector->x);
    printf(", ");
    vector->typeInfo->print(vector->y);
    printf(", ");
    vector->typeInfo->print(vector->z);
    printf(")\n");

    return VECTOR_OPERATION_OK;
}