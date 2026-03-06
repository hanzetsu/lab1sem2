#include <assert.h>
#include <stdio.h>
#include <math.h>
#include "LinearForm.h"
#include "Complex.h"

#define EPS 1e-9

static int double_eq(double a, double b)
{
    return fabs(a - b) < EPS;
}

static int complex_eq(const Complex *a, const Complex *b)
{
    return double_eq(a->real, b->real) && double_eq(a->imag, b->imag);
}

static void check_int_coeffs(const int *expected, const int *actual, uint32_t n)
{
    for (uint32_t i = 0; i <= n; i++)
    {
        assert(expected[i] == actual[i]);
    }
}

static void check_double_coeffs(const double *expected, const double *actual, uint32_t n)
{
    for (uint32_t i = 0; i <= n; i++)
    {
        assert(double_eq(expected[i], actual[i]));
    }
}

static void check_complex_coeffs(const Complex *expected, const Complex *actual, uint32_t n)
{
    for (uint32_t i = 0; i <= n; i++)
    {
        assert(complex_eq(&expected[i], &actual[i]));
    }
}

static void test_int_edge()
{
    puts("Тестирование целочисленных граничных случаев...");
    LinearFormErrors err;

    int coeffs[] = {1, 2, 3};
    LinearForm *lf = createLinearForm(GetIntTypeInfo(), coeffs, 2, &err);
    assert(err == LINEARFORM_OPERATION_OK);
    LinearForm *scaled = createLinearForm(GetIntTypeInfo(), NULL, 2, &err);
    assert(err == LINEARFORM_OPERATION_OK);
    int zero_scalar = 0;
    err = multiplyScalarLinearForm(lf, &zero_scalar, scaled);
    assert(err == LINEARFORM_OPERATION_OK);
    int expected_zero[] = {0, 0, 0};
    check_int_coeffs(expected_zero, (int *)scaled->coeffs, 2);
    freeLinearForm(scaled);

    int zero_coeffs[] = {0, 0, 0};
    LinearForm *zero_lf = createLinearForm(GetIntTypeInfo(), zero_coeffs, 2, &err);
    assert(err == LINEARFORM_OPERATION_OK);
    LinearForm *sum = createLinearForm(GetIntTypeInfo(), NULL, 2, &err);
    assert(err == LINEARFORM_OPERATION_OK);
    err = addLinearForms(lf, zero_lf, sum);
    assert(err == LINEARFORM_OPERATION_OK);
    check_int_coeffs(coeffs, (int *)sum->coeffs, 2);
    freeLinearForm(sum);
    freeLinearForm(zero_lf);

    LinearForm *diff = createLinearForm(GetIntTypeInfo(), NULL, 2, &err);
    assert(err == LINEARFORM_OPERATION_OK);
    err = subtractLinearForms(lf, zero_lf, diff);
    assert(err == LINEARFORM_OPERATION_OK);
    check_int_coeffs(coeffs, (int *)diff->coeffs, 2);
    freeLinearForm(diff);

    int args_zero[] = {0, 0};
    int result;
    err = evaluateLinearForm(lf, args_zero, &result);
    assert(err == LINEARFORM_OPERATION_OK);
    assert(result == 1); // a0

    int coeffs_n0[] = {42};
    LinearForm *lf_n0 = createLinearForm(GetIntTypeInfo(), coeffs_n0, 0, &err);
    assert(err == LINEARFORM_OPERATION_OK);

    LinearForm *sum_n0 = createLinearForm(GetIntTypeInfo(), NULL, 2, &err);
    assert(err == LINEARFORM_OPERATION_OK);
    err = addLinearForms(lf, lf_n0, sum_n0);
    assert(err == LINEARFORM_OPERATION_OK);
    int expected_sum_n0[] = {43, 2, 3};
    check_int_coeffs(expected_sum_n0, (int *)sum_n0->coeffs, 2);
    freeLinearForm(sum_n0);

    LinearForm *scaled_n0 = createLinearForm(GetIntTypeInfo(), NULL, 0, &err);
    assert(err == LINEARFORM_OPERATION_OK);
    err = multiplyScalarLinearForm(lf_n0, &zero_scalar, scaled_n0);
    assert(err == LINEARFORM_OPERATION_OK);
    assert(*(int *)scaled_n0->coeffs == 0);
    freeLinearForm(scaled_n0);

    int result_n0;
    err = evaluateLinearForm(lf_n0, NULL, &result_n0);
    assert(err == LINEARFORM_OPERATION_OK);
    assert(result_n0 == 42);
    freeLinearForm(lf_n0);

    freeLinearForm(lf);
    puts("Целочисленные прошли успешно");
}

static void test_double_edge()
{
    puts("Тестирование граничных случаев чисел с плавающей запятой ...");
    LinearFormErrors err;

    double coeffs[] = {1.0, 2.0, 3.0};
    LinearForm *lf = createLinearForm(GetDoubleTypeInfo(), coeffs, 2, &err);
    assert(err == LINEARFORM_OPERATION_OK);

    double zero_scalar = 0.0;
    LinearForm *scaled = createLinearForm(GetDoubleTypeInfo(), NULL, 2, &err);
    assert(err == LINEARFORM_OPERATION_OK);
    err = multiplyScalarLinearForm(lf, &zero_scalar, scaled);
    assert(err == LINEARFORM_OPERATION_OK);
    double expected_zero[] = {0.0, 0.0, 0.0};
    check_double_coeffs(expected_zero, (double *)scaled->coeffs, 2);
    freeLinearForm(scaled);

    double args_zero[] = {0.0, 0.0};
    double result;
    err = evaluateLinearForm(lf, args_zero, &result);
    assert(err == LINEARFORM_OPERATION_OK);
    assert(double_eq(result, 1.0));

    double coeffs_n0[] = {3.14};
    LinearForm *lf_n0 = createLinearForm(GetDoubleTypeInfo(), coeffs_n0, 0, &err);
    assert(err == LINEARFORM_OPERATION_OK);
    double result_n0;
    err = evaluateLinearForm(lf_n0, NULL, &result_n0);
    assert(err == LINEARFORM_OPERATION_OK);
    assert(double_eq(result_n0, 3.14));
    freeLinearForm(lf_n0);

    freeLinearForm(lf);
    puts("Успешное тестирование.");
}

static void test_complex_edge()
{
    puts("Тестирование граничных случаев комплексных чисел...");
    LinearFormErrors err;

    Complex coeffs[] = {{1, 0}, {2, 1}, {3, -1}};
    LinearForm *lf = createLinearForm(GetComplexTypeInfo(), coeffs, 2, &err);
    assert(err == LINEARFORM_OPERATION_OK);

    Complex zero_scalar = {0, 0};
    LinearForm *scaled = createLinearForm(GetComplexTypeInfo(), NULL, 2, &err);
    assert(err == LINEARFORM_OPERATION_OK);
    err = multiplyScalarLinearForm(lf, &zero_scalar, scaled);
    assert(err == LINEARFORM_OPERATION_OK);
    Complex expected_zero[] = {{0, 0}, {0, 0}, {0, 0}};
    check_complex_coeffs(expected_zero, (Complex *)scaled->coeffs, 2);
    freeLinearForm(scaled);

    Complex imag_unit = {0, 1};
    scaled = createLinearForm(GetComplexTypeInfo(), NULL, 2, &err);
    assert(err == LINEARFORM_OPERATION_OK);
    err = multiplyScalarLinearForm(lf, &imag_unit, scaled);
    assert(err == LINEARFORM_OPERATION_OK);
    Complex expected_imag[] = {{0, 1}, {-1, 2}, {1, 3}};
    check_complex_coeffs(expected_imag, (Complex *)scaled->coeffs, 2);
    freeLinearForm(scaled);

    Complex args_zero[] = {{0, 0}, {0, 0}};
    Complex result;
    err = evaluateLinearForm(lf, args_zero, &result);
    assert(err == LINEARFORM_OPERATION_OK);
    assert(complex_eq(&result, &coeffs[0])); // a0

    Complex coeffs_n0[] = {{42, -7}};
    LinearForm *lf_n0 = createLinearForm(GetComplexTypeInfo(), coeffs_n0, 0, &err);
    assert(err == LINEARFORM_OPERATION_OK);
    Complex result_n0;
    err = evaluateLinearForm(lf_n0, NULL, &result_n0);
    assert(err == LINEARFORM_OPERATION_OK);
    assert(complex_eq(&result_n0, &coeffs_n0[0]));
    freeLinearForm(lf_n0);

    freeLinearForm(lf);
    puts("Успешное тестирование комплексных чисел.");
}

int main()
{
    test_int_edge();
    test_double_edge();
    test_complex_edge();
    puts("Все тесты пройдены");
    return 0;
}