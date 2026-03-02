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
    for (uint64_t i = 0; i < max_n + 1; i++)
    {
        if (a->n >= i && b->n >= i)
            a->typeInfo->add((char *)a->coeffs + (i * a->typeInfo->size), (char *)b->coeffs + (i * b->typeInfo->size), (char *)result->coeffs + (i * result->typeInfo->size));
        if (a->n < i)
            a->typeInfo->add(0, b, result[i]);
        if (b->n < i)
            a->typeInfo->add(a, 0, result[i]);
    }
    return LINEARFORM_OPERATION_OK;
}

VectorErrors multiplyVectors(const Vector *a, const Vector *b, Vector *result)
{
    return VECTOR_OPERATION_OK;
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