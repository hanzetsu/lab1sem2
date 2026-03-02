#include "LinearForm.h"
#include "TypeInfo.h"

LinearForm* createLinearForm(TypeInfo* typeInfo, void* coeffs, uint64_t n, LinearFormErrors* operationResult) {
    LinearForm* lf = (LinearForm*)malloc(sizeof(*lf));
    if (lf == NULL) {
        *operationResult = MEMORY_ALLOCATION_FAILED;
        return NULL;
    }

    lf->typeInfo = typeInfo;
    lf->coeffs= malloc(typeInfo->size*lf->n);
    *operationResult = LINEARFORM_OPERATION_OK;
    return lf;
}

void freeLinearForm(LinearForm* lf) {
    free(lf->coeffs);
    free(lf); }

LinearFormErrors addLinearForms(const LinearForm* a, const LinearForm* b, LinearForm* result) {
    if (a == NULL || b == NULL || result == NULL) return LINEARFORM_NOT_DEFINED;
    uint64_t n = MAX(a->n,b->n);
    for (uint64_t i = 0; i < n+1; i++){
        if (a->typeInfo != b->typeInfo || a->typeInfo != result->typeInfo) return INCOMPATIBLE_LINEARFORM_TYPES;
        if (a->typeInfo. )
        result->coeffs[i] = a->coeffs[i] + b->coeffs[i];
    } // HZ
    return LINEARFORM_OPERATION_OK;
}

VectorErrors multiplyVectors(const Vector* a, const Vector* b, Vector* result) {
    return VECTOR_OPERATION_OK;
}

VectorErrors printVector(const Vector* vector) {
    if (vector == NULL) return VECTOR_NOT_DEFINED;
    if (vector->typeInfo->print == NULL) return OPERATION_NOT_DEFINED;

    printf("Vector: (");
    vector->typeInfo->print(vector->x);
    printf(", ");
    vector->typeInfo->print(vector->y);
    printf(", ");
    vector->typeInfo->print(vector->z);
    printf(")\n");

    return VECTOR_OPERATION_OK;
}