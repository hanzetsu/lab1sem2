#include <assert.h>
#include <stdio.h>
#include <math.h>
#include "LinearForm.h"
#include "Complex.h"

#define EPS 1e-9

static int double_eq(double a, double b) {
    return fabs(a - b) < EPS;
}

static int complex_eq(const Complex *a, const Complex *b) {
    return double_eq(a->real, b->real) && double_eq(a->imag, b->imag);
}

static void check_int_coeffs(const int *expected, const int *actual, uint32_t n) {
    for (uint32_t i = 0; i <= n; i++)
        assert(expected[i] == actual[i]);
}

static void check_double_coeffs(const double *expected, const double *actual, uint32_t n) {
    for (uint32_t i = 0; i <= n; i++)
        assert(double_eq(expected[i], actual[i]));
}

static void check_complex_coeffs(const Complex *expected, const Complex *actual, uint32_t n) {
    for (uint32_t i = 0; i <= n; i++)
        assert(complex_eq(&expected[i], &actual[i]));
}


static void test_int_sum_with_zero() {
    puts("  int: сложение с нулевой формой");
    LinearFormErrors err;
    int coeffs[] = {1, 2, 3};
    LinearForm *lf = createLinearForm(GetIntTypeInfo(), coeffs, 2, &err);
    assert(err == LINEARFORM_OPERATION_OK);

    int zero_coeffs[] = {0, 0, 0};
    LinearForm *zero = createLinearForm(GetIntTypeInfo(), zero_coeffs, 2, &err);
    assert(err == LINEARFORM_OPERATION_OK);

    LinearForm *sum = createLinearForm(GetIntTypeInfo(), NULL, 2, &err);
    assert(err == LINEARFORM_OPERATION_OK);

    err = sumLinearForms(lf, zero, sum);
    assert(err == LINEARFORM_OPERATION_OK);
    check_int_coeffs(coeffs, (int*)sum->coeffs, 2);

    freeLinearForm(lf);
    freeLinearForm(zero);
    freeLinearForm(sum);
}

static void test_int_subtract_with_zero() {
    puts("  int: вычитание нулевой формы");
    LinearFormErrors err;
    int coeffs[] = {1, 2, 3};
    LinearForm *lf = createLinearForm(GetIntTypeInfo(), coeffs, 2, &err);
    assert(err == LINEARFORM_OPERATION_OK);

    int zero_coeffs[] = {0, 0, 0};
    LinearForm *zero = createLinearForm(GetIntTypeInfo(), zero_coeffs, 2, &err);
    assert(err == LINEARFORM_OPERATION_OK);

    LinearForm *diff = createLinearForm(GetIntTypeInfo(), NULL, 2, &err);
    assert(err == LINEARFORM_OPERATION_OK);

    err = subtractLinearForms(lf, zero, diff);
    assert(err == LINEARFORM_OPERATION_OK);
    check_int_coeffs(coeffs, (int*)diff->coeffs, 2);

    freeLinearForm(lf);
    freeLinearForm(zero);
    freeLinearForm(diff);
}

static void test_int_multiply_by_zero() {
    puts("  int: умножение на нулевой скаляр");
    LinearFormErrors err;
    int coeffs[] = {1, 2, 3};
    LinearForm *lf = createLinearForm(GetIntTypeInfo(), coeffs, 2, &err);
    assert(err == LINEARFORM_OPERATION_OK);

    LinearForm *scaled = createLinearForm(GetIntTypeInfo(), NULL, 2, &err);
    assert(err == LINEARFORM_OPERATION_OK);

    int zero_scalar = 0;
    err = multiplyScalarLinearForm(lf, &zero_scalar, scaled);
    assert(err == LINEARFORM_OPERATION_OK);

    int expected[] = {0, 0, 0};
    check_int_coeffs(expected, (int*)scaled->coeffs, 2);

    freeLinearForm(lf);
    freeLinearForm(scaled);
}

static void test_int_evaluate_zero_args() {
    puts("  int: вычисление при нулевых аргументах");
    LinearFormErrors err;
    int coeffs[] = {5, -2, 3};
    LinearForm *lf = createLinearForm(GetIntTypeInfo(), coeffs, 2, &err);
    assert(err == LINEARFORM_OPERATION_OK);

    int args[] = {0, 0};
    int result;
    err = evaluateLinearForm(lf, args, &result);
    assert(err == LINEARFORM_OPERATION_OK);
    assert(result == 5); 
    freeLinearForm(lf);
}

static void test_int_form_with_n0() {
    puts("  int: форма с n = 0");
    LinearFormErrors err;
    int coeffs[] = {42};
    LinearForm *lf = createLinearForm(GetIntTypeInfo(), coeffs, 0, &err);
    assert(err == LINEARFORM_OPERATION_OK);

    LinearForm *scaled = createLinearForm(GetIntTypeInfo(), NULL, 0, &err);
    assert(err == LINEARFORM_OPERATION_OK);
    int scalar = 2;
    err = multiplyScalarLinearForm(lf, &scalar, scaled);
    assert(err == LINEARFORM_OPERATION_OK);
    assert(*(int*)scaled->coeffs == 84);
    freeLinearForm(scaled);

    int result;
    err = evaluateLinearForm(lf, NULL, &result);
    assert(err == LINEARFORM_OPERATION_OK);
    assert(result == 42);

    freeLinearForm(lf);
}

static void test_int_sum_diff_dimensions() {
    puts("  int: сложение форм разной размерности");
    LinearFormErrors err;
    int coeffs_a[] = {1, 2};    
    int coeffs_b[] = {3, 4, 5};   
    LinearForm *a = createLinearForm(GetIntTypeInfo(), coeffs_a, 1, &err);
    assert(err == LINEARFORM_OPERATION_OK);
    LinearForm *b = createLinearForm(GetIntTypeInfo(), coeffs_b, 2, &err);
    assert(err == LINEARFORM_OPERATION_OK);

    LinearForm *sum = createLinearForm(GetIntTypeInfo(), NULL, 2, &err);
    assert(err == LINEARFORM_OPERATION_OK);

    err = sumLinearForms(a, b, sum);
    assert(err == LINEARFORM_OPERATION_OK);

    int expected[] = {1+3, 2+4, 0+5};
    check_int_coeffs(expected, (int*)sum->coeffs, 2);

    freeLinearForm(a);
    freeLinearForm(b);
    freeLinearForm(sum);
}


static void test_double_sum_with_zero() {
    puts("  double: сложение с нулевой формой");
    LinearFormErrors err;
    double coeffs[] = {1.5, -2.3, 3.7};
    LinearForm *lf = createLinearForm(GetDoubleTypeInfo(), coeffs, 2, &err);
    assert(err == LINEARFORM_OPERATION_OK);

    double zero_coeffs[] = {0.0, 0.0, 0.0};
    LinearForm *zero = createLinearForm(GetDoubleTypeInfo(), zero_coeffs, 2, &err);
    assert(err == LINEARFORM_OPERATION_OK);

    LinearForm *sum = createLinearForm(GetDoubleTypeInfo(), NULL, 2, &err);
    assert(err == LINEARFORM_OPERATION_OK);

    err = sumLinearForms(lf, zero, sum);
    assert(err == LINEARFORM_OPERATION_OK);
    check_double_coeffs(coeffs, (double*)sum->coeffs, 2);

    freeLinearForm(lf);
    freeLinearForm(zero);
    freeLinearForm(sum);
}

static void test_double_subtract_with_zero() {
    puts("  double: вычитание нулевой формы");
    LinearFormErrors err;
    double coeffs[] = {1.5, -2.3, 3.7};
    LinearForm *lf = createLinearForm(GetDoubleTypeInfo(), coeffs, 2, &err);
    assert(err == LINEARFORM_OPERATION_OK);

    double zero_coeffs[] = {0.0, 0.0, 0.0};
    LinearForm *zero = createLinearForm(GetDoubleTypeInfo(), zero_coeffs, 2, &err);
    assert(err == LINEARFORM_OPERATION_OK);

    LinearForm *diff = createLinearForm(GetDoubleTypeInfo(), NULL, 2, &err);
    assert(err == LINEARFORM_OPERATION_OK);

    err = subtractLinearForms(lf, zero, diff);
    assert(err == LINEARFORM_OPERATION_OK);
    check_double_coeffs(coeffs, (double*)diff->coeffs, 2);

    freeLinearForm(lf);
    freeLinearForm(zero);
    freeLinearForm(diff);
}

static void test_double_multiply_by_zero() {
    puts("  double: умножение на нулевой скаляр");
    LinearFormErrors err;
    double coeffs[] = {1.5, -2.3, 3.7};
    LinearForm *lf = createLinearForm(GetDoubleTypeInfo(), coeffs, 2, &err);
    assert(err == LINEARFORM_OPERATION_OK);

    LinearForm *scaled = createLinearForm(GetDoubleTypeInfo(), NULL, 2, &err);
    assert(err == LINEARFORM_OPERATION_OK);

    double zero_scalar = 0.0;
    err = multiplyScalarLinearForm(lf, &zero_scalar, scaled);
    assert(err == LINEARFORM_OPERATION_OK);

    double expected[] = {0.0, 0.0, 0.0};
    check_double_coeffs(expected, (double*)scaled->coeffs, 2);

    freeLinearForm(lf);
    freeLinearForm(scaled);
}

static void test_double_evaluate_zero_args() {
    puts("  double: вычисление при нулевых аргументах");
    LinearFormErrors err;
    double coeffs[] = {2.5, -1.2, 0.8};
    LinearForm *lf = createLinearForm(GetDoubleTypeInfo(), coeffs, 2, &err);
    assert(err == LINEARFORM_OPERATION_OK);

    double args[] = {0.0, 0.0};
    double result;
    err = evaluateLinearForm(lf, args, &result);
    assert(err == LINEARFORM_OPERATION_OK);
    assert(double_eq(result, 2.5));

    freeLinearForm(lf);
}

static void test_double_form_with_n0() {
    puts("  double: форма с n = 0");
    LinearFormErrors err;
    double coeffs[] = {3.14};
    LinearForm *lf = createLinearForm(GetDoubleTypeInfo(), coeffs, 0, &err);
    assert(err == LINEARFORM_OPERATION_OK);

    LinearForm *scaled = createLinearForm(GetDoubleTypeInfo(), NULL, 0, &err);
    assert(err == LINEARFORM_OPERATION_OK);
    double scalar = 2.0;
    err = multiplyScalarLinearForm(lf, &scalar, scaled);
    assert(err == LINEARFORM_OPERATION_OK);
    assert(double_eq(*(double*)scaled->coeffs, 6.28));
    freeLinearForm(scaled);

    double result;
    err = evaluateLinearForm(lf, NULL, &result);
    assert(err == LINEARFORM_OPERATION_OK);
    assert(double_eq(result, 3.14));

    freeLinearForm(lf);
}


static void test_complex_sum_with_zero() {
    puts("  complex: сложение с нулевой формой");
    LinearFormErrors err;
    Complex coeffs[] = {{1,0}, {2,1}, {3,-1}};
    LinearForm *lf = createLinearForm(GetComplexTypeInfo(), coeffs, 2, &err);
    assert(err == LINEARFORM_OPERATION_OK);

    Complex zero_coeffs[] = {{0,0}, {0,0}, {0,0}};
    LinearForm *zero = createLinearForm(GetComplexTypeInfo(), zero_coeffs, 2, &err);
    assert(err == LINEARFORM_OPERATION_OK);

    LinearForm *sum = createLinearForm(GetComplexTypeInfo(), NULL, 2, &err);
    assert(err == LINEARFORM_OPERATION_OK);

    err = sumLinearForms(lf, zero, sum);
    assert(err == LINEARFORM_OPERATION_OK);
    check_complex_coeffs(coeffs, (Complex*)sum->coeffs, 2);

    freeLinearForm(lf);
    freeLinearForm(zero);
    freeLinearForm(sum);
}

static void test_complex_subtract_with_zero() {
    puts("  complex: вычитание нулевой формы");
    LinearFormErrors err;
    Complex coeffs[] = {{1,0}, {2,1}, {3,-1}};
    LinearForm *lf = createLinearForm(GetComplexTypeInfo(), coeffs, 2, &err);
    assert(err == LINEARFORM_OPERATION_OK);

    Complex zero_coeffs[] = {{0,0}, {0,0}, {0,0}};
    LinearForm *zero = createLinearForm(GetComplexTypeInfo(), zero_coeffs, 2, &err);
    assert(err == LINEARFORM_OPERATION_OK);

    LinearForm *diff = createLinearForm(GetComplexTypeInfo(), NULL, 2, &err);
    assert(err == LINEARFORM_OPERATION_OK);

    err = subtractLinearForms(lf, zero, diff);
    assert(err == LINEARFORM_OPERATION_OK);
    check_complex_coeffs(coeffs, (Complex*)diff->coeffs, 2);

    freeLinearForm(lf);
    freeLinearForm(zero);
    freeLinearForm(diff);
}

static void test_complex_multiply_by_zero() {
    puts("  complex: умножение на нулевой скаляр");
    LinearFormErrors err;
    Complex coeffs[] = {{1,0}, {2,1}, {3,-1}};
    LinearForm *lf = createLinearForm(GetComplexTypeInfo(), coeffs, 2, &err);
    assert(err == LINEARFORM_OPERATION_OK);

    LinearForm *scaled = createLinearForm(GetComplexTypeInfo(), NULL, 2, &err);
    assert(err == LINEARFORM_OPERATION_OK);

    Complex zero_scalar = {0,0};
    err = multiplyScalarLinearForm(lf, &zero_scalar, scaled);
    assert(err == LINEARFORM_OPERATION_OK);

    Complex expected[] = {{0,0}, {0,0}, {0,0}};
    check_complex_coeffs(expected, (Complex*)scaled->coeffs, 2);

    freeLinearForm(lf);
    freeLinearForm(scaled);
}

static void test_complex_multiply_by_imag_unit() {
    puts("  complex: умножение на мнимую единицу");
    LinearFormErrors err;
    Complex coeffs[] = {{1,0}, {2,1}, {3,-1}};
    LinearForm *lf = createLinearForm(GetComplexTypeInfo(), coeffs, 2, &err);
    assert(err == LINEARFORM_OPERATION_OK);

    LinearForm *scaled = createLinearForm(GetComplexTypeInfo(), NULL, 2, &err);
    assert(err == LINEARFORM_OPERATION_OK);

    Complex imag_unit = {0,1};
    err = multiplyScalarLinearForm(lf, &imag_unit, scaled);
    assert(err == LINEARFORM_OPERATION_OK);

    Complex expected[] = {{0,1}, {-1,2}, {1,3}};
    check_complex_coeffs(expected, (Complex*)scaled->coeffs, 2);

    freeLinearForm(lf);
    freeLinearForm(scaled);
}

static void test_complex_evaluate_zero_args() {
    puts("  complex: вычисление при нулевых аргументах");
    LinearFormErrors err;
    Complex coeffs[] = {{5, -3}, {1,2}, {0,4}};
    LinearForm *lf = createLinearForm(GetComplexTypeInfo(), coeffs, 2, &err);
    assert(err == LINEARFORM_OPERATION_OK);

    Complex args[] = {{0,0}, {0,0}};
    Complex result;
    err = evaluateLinearForm(lf, args, &result);
    assert(err == LINEARFORM_OPERATION_OK);
    assert(complex_eq(&result, &coeffs[0]));

    freeLinearForm(lf);
}

static void test_complex_form_with_n0() {
    puts("  complex: форма с n = 0");
    LinearFormErrors err;
    Complex coeffs[] = {{42, -7}};
    LinearForm *lf = createLinearForm(GetComplexTypeInfo(), coeffs, 0, &err);
    assert(err == LINEARFORM_OPERATION_OK);

    LinearForm *scaled = createLinearForm(GetComplexTypeInfo(), NULL, 0, &err);
    assert(err == LINEARFORM_OPERATION_OK);
    Complex scalar = {2, 3};
    err = multiplyScalarLinearForm(lf, &scalar, scaled);
    assert(err == LINEARFORM_OPERATION_OK);
    Complex expected = {2*42 - 3*(-7), 2*(-7) + 3*42};
    Complex actual = *(Complex*)scaled->coeffs;
    assert(complex_eq(&actual, &expected));
    freeLinearForm(scaled);

    Complex result;
    err = evaluateLinearForm(lf, NULL, &result);
    assert(err == LINEARFORM_OPERATION_OK);
    assert(complex_eq(&result, &coeffs[0]));

    freeLinearForm(lf);
}

int main() {
    puts("Запуск тестов");

    puts("\nЦелочисленный тип");
    test_int_sum_with_zero();
    test_int_subtract_with_zero();
    test_int_multiply_by_zero();
    test_int_evaluate_zero_args();
    test_int_form_with_n0();
    test_int_sum_diff_dimensions();

    puts("\nВещественный тип");
    test_double_sum_with_zero();
    test_double_subtract_with_zero();
    test_double_multiply_by_zero();
    test_double_evaluate_zero_args();
    test_double_form_with_n0();

    puts("\nКомплексный тип");
    test_complex_sum_with_zero();
    test_complex_subtract_with_zero();
    test_complex_multiply_by_zero();
    test_complex_multiply_by_imag_unit();
    test_complex_evaluate_zero_args();
    test_complex_form_with_n0();

    puts("\nВсе тесты пройдены!");
    return 0;
}