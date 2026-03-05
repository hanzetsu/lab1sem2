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

// Тест краевых случаев для int
static void test_int_edge()
{
    printf("Тестирование целочисленных граничных случаев...\n");
    LinearFormErrors err;

    // 1. Умножение на нулевой скаляр
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

    // 2. Сложение с нулевой формой
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

    // 3. Вычитание нулевой формы
    LinearForm *diff = createLinearForm(GetIntTypeInfo(), NULL, 2, &err);
    assert(err == LINEARFORM_OPERATION_OK);
    err = subtractLinearForms(lf, zero_lf, diff);
    assert(err == LINEARFORM_OPERATION_OK);
    check_int_coeffs(coeffs, (int *)diff->coeffs, 2);
    freeLinearForm(diff);

    // 4. Вычисление значения с нулевыми аргументами
    int args_zero[] = {0, 0};
    int result;
    err = evaluateLinearForm(lf, args_zero, &result);
    assert(err == LINEARFORM_OPERATION_OK);
    assert(result == 1); // a0

    // 5. Форма с n=0 (только свободный член)
    int coeffs_n0[] = {42};
    LinearForm *lf_n0 = createLinearForm(GetIntTypeInfo(), coeffs_n0, 0, &err);
    assert(err == LINEARFORM_OPERATION_OK);
    // Сложение с n=0 и n=2 должно дать max_n=2
    LinearForm *sum_n0 = createLinearForm(GetIntTypeInfo(), NULL, 2, &err);
    assert(err == LINEARFORM_OPERATION_OK);
    err = addLinearForms(lf, lf_n0, sum_n0);
    assert(err == LINEARFORM_OPERATION_OK);
    int expected_sum_n0[] = {43, 2, 3};
    check_int_coeffs(expected_sum_n0, (int *)sum_n0->coeffs, 2);
    freeLinearForm(sum_n0);
    // Умножение на скаляр для n=0
    LinearForm *scaled_n0 = createLinearForm(GetIntTypeInfo(), NULL, 0, &err);
    assert(err == LINEARFORM_OPERATION_OK);
    err = multiplyScalarLinearForm(lf_n0, &zero_scalar, scaled_n0);
    assert(err == LINEARFORM_OPERATION_OK);
    assert(*(int *)scaled_n0->coeffs == 0);
    freeLinearForm(scaled_n0);
    // Вычисление значения для n=0
    int result_n0;
    err = evaluateLinearForm(lf_n0, NULL, &result_n0); // аргументы не нужны
    assert(err == LINEARFORM_OPERATION_OK);
    assert(result_n0 == 42);
    freeLinearForm(lf_n0);

    // 6. Передача NULL
    err = addLinearForms(NULL, lf, lf);
    assert(err == LINEARFORM_NOT_DEFINED);
    err = subtractLinearForms(lf, NULL, lf);
    assert(err == LINEARFORM_NOT_DEFINED);
    err = multiplyScalarLinearForm(lf, &zero_scalar, NULL);
    assert(err == LINEARFORM_NOT_DEFINED);
    err = evaluateLinearForm(NULL, args_zero, &result);
    assert(err == LINEARFORM_NOT_DEFINED);

    // 8. Размерность результата больше максимальной
    LinearForm *result_too_big = createLinearForm(GetIntTypeInfo(), NULL, 3, &err); // n=3
    assert(err == LINEARFORM_OPERATION_OK);
    err = addLinearForms(lf, lf, result_too_big);
    assert(err == DIMENSION_MISMATCH); // т.к. max_n=2, а result->n=3 -> mismatch
    freeLinearForm(result_too_big);

    freeLinearForm(lf);
    puts("Целочисленные прошли успешно");
}

static void test_double_edge()
{
    printf("Тестирование граничных случаев чисел с плавающей запятой ...\n");
    LinearFormErrors err;

    double coeffs[] = {1.0, 2.0, 3.0};
    LinearForm *lf = createLinearForm(GetDoubleTypeInfo(), coeffs, 2, &err);
    assert(err == LINEARFORM_OPERATION_OK);

    // Умножение на ноль
    double zero_scalar = 0.0;
    LinearForm *scaled = createLinearForm(GetDoubleTypeInfo(), NULL, 2, &err);
    assert(err == LINEARFORM_OPERATION_OK);
    err = multiplyScalarLinearForm(lf, &zero_scalar, scaled);
    assert(err == LINEARFORM_OPERATION_OK);
    double expected_zero[] = {0.0, 0.0, 0.0};
    check_double_coeffs(expected_zero, (double *)scaled->coeffs, 2);
    freeLinearForm(scaled);

    // Вычисление значения с нулевыми аргументами
    double args_zero[] = {0.0, 0.0};
    double result;
    err = evaluateLinearForm(lf, args_zero, &result);
    assert(err == LINEARFORM_OPERATION_OK);
    assert(double_eq(result, 1.0));

    // Форма n=0
    double coeffs_n0[] = {3.14};
    LinearForm *lf_n0 = createLinearForm(GetDoubleTypeInfo(), coeffs_n0, 0, &err);
    assert(err == LINEARFORM_OPERATION_OK);
    double result_n0;
    err = evaluateLinearForm(lf_n0, NULL, &result_n0);
    assert(err == LINEARFORM_OPERATION_OK);
    assert(double_eq(result_n0, 3.14));
    freeLinearForm(lf_n0);

    freeLinearForm(lf);
    printf("Успешное тестирование.\n");
}

// Для complex
static void test_complex_edge()
{
    printf("Тестирование граничных случаев комплексных чисел...\n");
    LinearFormErrors err;

    Complex coeffs[] = {{1, 0}, {2, 1}, {3, -1}};
    LinearForm *lf = createLinearForm(GetComplexTypeInfo(), coeffs, 2, &err);
    assert(err == LINEARFORM_OPERATION_OK);

    // Умножение на нулевой комплексный скаляр
    Complex zero_scalar = {0, 0};
    LinearForm *scaled = createLinearForm(GetComplexTypeInfo(), NULL, 2, &err);
    assert(err == LINEARFORM_OPERATION_OK);
    err = multiplyScalarLinearForm(lf, &zero_scalar, scaled);
    assert(err == LINEARFORM_OPERATION_OK);
    Complex expected_zero[] = {{0, 0}, {0, 0}, {0, 0}};
    check_complex_coeffs(expected_zero, (Complex *)scaled->coeffs, 2);
    freeLinearForm(scaled);

    // Умножение на чисто мнимую единицу
    Complex imag_unit = {0, 1};
    scaled = createLinearForm(GetComplexTypeInfo(), NULL, 2, &err);
    assert(err == LINEARFORM_OPERATION_OK);
    err = multiplyScalarLinearForm(lf, &imag_unit, scaled);
    assert(err == LINEARFORM_OPERATION_OK);
    // Ожидаемые результаты: (1,0)*i = (0,1); (2,1)*i = (-1,2); (3,-1)*i = (1,3)
    Complex expected_imag[] = {{0, 1}, {-1, 2}, {1, 3}};
    check_complex_coeffs(expected_imag, (Complex *)scaled->coeffs, 2);
    freeLinearForm(scaled);

    // Вычисление значения с нулевыми аргументами
    Complex args_zero[] = {{0, 0}, {0, 0}};
    Complex result;
    err = evaluateLinearForm(lf, args_zero, &result);
    assert(err == LINEARFORM_OPERATION_OK);
    assert(complex_eq(&result, &coeffs[0])); // a0

    // Форма n=0
    Complex coeffs_n0[] = {{42, -7}};
    LinearForm *lf_n0 = createLinearForm(GetComplexTypeInfo(), coeffs_n0, 0, &err);
    assert(err == LINEARFORM_OPERATION_OK);
    Complex result_n0;
    err = evaluateLinearForm(lf_n0, NULL, &result_n0);
    assert(err == LINEARFORM_OPERATION_OK);
    assert(complex_eq(&result_n0, &coeffs_n0[0]));
    freeLinearForm(lf_n0);

    freeLinearForm(lf);
    printf("Успешное тестирование комплексных чисел.\n");
}

int main()
{
    test_int_edge();
    test_double_edge();
    test_complex_edge();
    printf("All edge tests passed successfully!\n");
    return 0;
}