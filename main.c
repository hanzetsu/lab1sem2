#include "LinearForm.h"
#include "Complex.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    LinearFormErrors operationResult;
    puts("Введите кол-во линейных форм:");
    int number_of_LinearForms = 0;
    scanf("%d", &number_of_LinearForms);
    if (number_of_LinearForms <= 0)
    {
        puts("Число должно быть больше 0");
        return 1;
    }
    LinearForm **LinearForms = (LinearForm**)malloc(sizeof(LinearForm*) * number_of_LinearForms);
    if (!LinearForms) {
        puts("Ошибка выделения памяти");
        return 1;
    }

    for (uint32_t i = 0; i < number_of_LinearForms; i++)
    {
        printf("Выберите тип элементов для %u-ой линейной формы: 1 - целые, 2 - вещественные числа с плавающей запятой, 3 - комплексные\n", i);
        uint32_t type_of_elements = 0;
        for (;;)
        {
            if ((scanf("%u", &type_of_elements)) != 1 || (type_of_elements != 1 && type_of_elements != 2 && type_of_elements != 3))
            {
                puts("Неверный тип, выберите целое число от 1 до 3");
                fflush(stdin);
            }
            else
                break;
        }
        TypeInfo *typeinfo = NULL;
        switch (type_of_elements)
        {
        case 1:
            typeinfo = GetIntTypeInfo();
            break;
        case 2:
            typeinfo = GetDoubleTypeInfo();
            break;
        case 3:
            typeinfo = GetComplexTypeInfo();
            break;
        }
        if (!typeinfo) {
            printf("Ошибка получения TypeInfo\n");
            for (uint32_t k = 0; k < i; k++) freeLinearForm(LinearForms[k]);
            free(LinearForms);
            return 1;
        }

        printf("Введите кол-во переменных для %u-ой линейной формы: ", i);
        uint32_t n = 0;
        scanf("%u", &n);
        fflush(stdin);

        uint32_t number_of_coeffs = n + 1;
        void *temp = malloc(number_of_coeffs * typeinfo->size);
        if (!temp) {
            puts("Ошибка выделения временной памяти");
            for (uint32_t k = 0; k < i; k++) freeLinearForm(LinearForms[k]);
            free(LinearForms);
            return 1;
        }

        for (uint32_t j = 0; j < number_of_coeffs; j++)
        {
            printf("Введите %u коэффициент для %u-ой линейной формы: ", j, i);
            typeinfo->scan((char *)temp + j * typeinfo->size);
            fflush(stdin);
        }

        LinearForms[i] = createLinearForm(typeinfo, temp, n, &operationResult);
        free(temp);
        if (operationResult != LINEARFORM_OPERATION_OK)
        {
            printf("Ошибка создания формы: %d\n", operationResult);
            for (uint32_t k = 0; k <= i; k++)
                freeLinearForm(LinearForms[k]);
            free(LinearForms);
            return 1;
        }
    }

    puts("\nСозданные линейные формы:");
    for (uint32_t i = 0; i < number_of_LinearForms; i++)
    {
        printf("Форма %u: ", i);
        printLinearForm(LinearForms[i]);
    }

    int choice;
    do {
        puts("\nВыберите операцию:");
        puts("1 - сложение двух форм");
        puts("2 - вычитание двух форм");
        puts("3 - умножение формы на скаляр");
        puts("4 - вычисление значения формы");
        puts("0 - выход");
        printf("Ваш выбор: ");
        if (scanf("%d", &choice) != 1) {
            puts("Ошибка ввода.");
            fflush(stdin);
            choice = -1;
            continue;
        }
        fflush(stdin);

        if (choice == 0) break;

        if (choice == 1 || choice == 2) {
            int idx1, idx2;
            printf("Введите индексы первой и второй формы (0..%u): ", number_of_LinearForms-1);
            if (scanf("%d%d", &idx1, &idx2) != 2) {
                puts("Ошибка ввода.");
                fflush(stdin);
                continue;
            }
            fflush(stdin);
            if (idx1 < 0 || idx1 >= number_of_LinearForms || idx2 < 0 || idx2 >= number_of_LinearForms) {
                puts("Неверный индекс");
                continue;
            }
            const LinearForm *a = LinearForms[idx1];
            const LinearForm *b = LinearForms[idx2];
            if (a->typeInfo != b->typeInfo) {
                puts("Типы форм не совпадают");
                continue;
            }
            uint32_t max_n = MAX(a->n, b->n);
            LinearForm *res = createLinearForm(a->typeInfo, NULL, max_n, &operationResult);
            if (operationResult != LINEARFORM_OPERATION_OK) {
                puts("Ошибка создания результирующей формы");
                continue;
            }
            LinearFormErrors err;
            if (choice == 1)
                err = addLinearForms(a, b, res);
            else
                err = subtractLinearForms(a, b, res);
            if (err != LINEARFORM_OPERATION_OK) {
                printf("Ошибка операции: %d\n", err);
                freeLinearForm(res);
            } else {
                printf("Результат: ");
                printLinearForm(res);
                freeLinearForm(res);
            }
        }
        else if (choice == 3) {
            int idx;
            printf("Введите индекс формы (0..%u): ", number_of_LinearForms-1);
            if (scanf("%d", &idx) != 1) {
                puts("Ошибка ввода.");
                fflush(stdin);
                continue;
            }
            fflush(stdin);
            if (idx < 0 || idx >= number_of_LinearForms) {
                puts("Неверный индекс");
                continue;
            }
            const LinearForm *lf = LinearForms[idx];
            void *scalar = malloc(lf->typeInfo->size);
            if (!scalar) {
                puts("Ошибка выделения памяти.");
                continue;
            }
            printf("Введите скаляр: ");
            lf->typeInfo->scan(scalar);
            fflush(stdin);

            LinearForm *res = createLinearForm(lf->typeInfo, NULL, lf->n, &operationResult);
            if (operationResult != LINEARFORM_OPERATION_OK) {
                puts("Ошибка создания результирующей формы");
                free(scalar);
                continue;
            }
            LinearFormErrors err = multiplyScalarLinearForm(lf, scalar, res);
            if (err != LINEARFORM_OPERATION_OK) {
                printf("Ошибка операции: %d\n", err);
                freeLinearForm(res);
            } else {
                printf("Результат: ");
                printLinearForm(res);
                freeLinearForm(res);
            }
            free(scalar);
        }
        else if (choice == 4) {
            int idx;
            printf("Введите индекс формы (0..%u): ", number_of_LinearForms-1);
            if (scanf("%d", &idx) != 1) {
                puts("Ошибка ввода.");
                fflush(stdin);
                continue;
            }
            fflush(stdin);
            if (idx < 0 || idx >= number_of_LinearForms) {
                puts("Неверный индекс");
                continue;
            }
            const LinearForm *lf = LinearForms[idx];
            void *args = malloc(lf->n * lf->typeInfo->size);
            if (!args) {
                puts("Ошибка выделения памяти.");
                continue;
            }
            for (uint32_t i = 0; i < lf->n; i++) {
                printf("Введите значение x%u: ", i+1);
                lf->typeInfo->scan((char*)args + i * lf->typeInfo->size);
                fflush(stdin);
            }
            void *result = malloc(lf->typeInfo->size);
            if (!result) {
                puts("Ошибка выделения памяти.");
                free(args);
                continue;
            }
            LinearFormErrors err = evaluateLinearForm(lf, args, result);
            if (err != LINEARFORM_OPERATION_OK) {
                printf("Ошибка вычисления: %d\n", err);
            } else {
                printf("Значение формы: ");
                lf->typeInfo->print(result);
                putchar('\n');
            }
            free(args);
            free(result);
        }
        else {
            puts("Неверный выбор");
        }
    } while (choice != 0);

    for (int i = 0; i < number_of_LinearForms; i++)
    {
        freeLinearForm(LinearForms[i]);
    }
    free(LinearForms);

    return 0;
}