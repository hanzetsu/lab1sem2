#include "interface.h"
#include "LinearForm.h"
#include "Complex.h"
#include <stdio.h>
#include <stdlib.h>

static void clear_input() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void run_interface()
{
    LinearFormErrors operationResult;
    LinearForm *forms[2] = {NULL, NULL};

    puts("Создание двух линейных форм.");

    for (int i = 0; i < 2; ++i)
    {
        uint32_t idx = i + 1;
        printf("\n--- Форма %u ---\n", idx);

        printf("Выберите тип элементов: 1 - целые, 2 - вещественные, 3 - комплексные: ");
        uint32_t type;
        char next;
        for (;;)
        {
            if (scanf("%u%c", &type, &next) == 2 && (next == '\n' || next == ' ') && type >= 1 && type <= 3)
            {
                if (next == ' ') clear_input();
                break;
            }
            else
            {
                puts("Неверный тип. Введите 1, 2 или 3.");
                clear_input();
            }
        }

        TypeInfo *ti = NULL;
        switch (type)
        {
        case 1:
            ti = GetIntTypeInfo();
            break;
        case 2:
            ti = GetDoubleTypeInfo();
            break;
        case 3:
            ti = GetComplexTypeInfo();
            break;
        }
        if (!ti)
        {
            printf("Ошибка получения TypeInfo\n");
            for (int k = 0; k < i; k++)
                freeLinearForm(forms[k]);
            return;
        }

        printf("Введите количество переменных (n): ");
        uint32_t n;
        for (;;)
        {
            if (scanf("%u%c", &n, &next) == 2 && (next == '\n' || next == ' '))
            {
                if (next == ' ') clear_input();
                break;
            }
            else
            {
                puts("Неверный ввод. Введите натуральное число");
                clear_input();
            }
        }

        uint32_t coeff_count = n + 1;
        void *temp = malloc(coeff_count * ti->size);
        if (!temp)
        {
            puts("Ошибка выделения временной памяти");
            for (int k = 0; k < i; k++)
                freeLinearForm(forms[k]);
            return;
        }

        for (uint32_t j = 1; j <= coeff_count; ++j)
        {
            printf("Введите коэффициент x_%u: ", j - 1);
            ti->scan((char *)temp + (j - 1) * ti->size);
        }

        forms[i] = createLinearForm(ti, temp, n, &operationResult);
        free(temp);

        if (operationResult != LINEARFORM_OPERATION_OK)
        {
            printf("Ошибка создания формы: %d\n", operationResult);
            for (int k = 0; k <= i; k++)
                freeLinearForm(forms[k]);
            return;
        }
    }

    puts("\nСозданные линейные формы:");
    for (int i = 0; i < 2; ++i)
    {
        printf("Форма %d: ", i + 1);
        printLinearForm(forms[i]);
    }

    int choice;
    do
    {
        puts("\nВыберите операцию:");
        puts("1 - сложение двух форм");
        puts("2 - вычитание двух форм");
        puts("3 - умножение формы на скаляр");
        puts("4 - вычисление значения формы");
        puts("0 - выход");
        printf("Ваш выбор: ");

        char next;
        if (scanf("%d%c", &choice, &next) != 2 || (next != '\n' && next != ' '))
        {
            puts("Ошибка ввода.");
            clear_input();
            choice = -1;
            continue;
        }
        if (next == ' ') clear_input();

        if (choice == 0)
            break;

        if (choice == 1 || choice == 2)
        {
            int num1, num2;
            printf("Введите номера первой и второй формы (1 или 2): ");
            if (scanf("%d%d%c", &num1, &num2, &next) != 3 || (next != '\n' && next != ' '))
            {
                puts("Ошибка ввода.");
                clear_input();
                continue;
            }
            if (next == ' ') clear_input();

            int idx1 = num1 - 1, idx2 = num2 - 1;
            if (idx1 < 0 || idx1 > 1 || idx2 < 0 || idx2 > 1)
            {
                puts("Неверный номер формы");
                continue;
            }
            const LinearForm *a = forms[idx1];
            const LinearForm *b = forms[idx2];
            if (a->typeInfo != b->typeInfo)
            {
                puts("Типы форм не совпадают");
                continue;
            }
            uint32_t max_n = MAX(a->n, b->n);
            LinearForm *res = createLinearForm(a->typeInfo, NULL, max_n, &operationResult);
            if (operationResult != LINEARFORM_OPERATION_OK)
            {
                puts("Ошибка создания результирующей формы");
                continue;
            }
            LinearFormErrors err;
            if (choice == 1)
                err = sumLinearForms(a, b, res);
            else
                err = subtractLinearForms(a, b, res);
            if (err != LINEARFORM_OPERATION_OK)
            {
                printf("Ошибка операции: %d\n", err);
                freeLinearForm(res);
            }
            else
            {
                printf("Результат: ");
                printLinearForm(res);
                freeLinearForm(res);
            }
        }
        else if (choice == 3)
        {
            int num;
            printf("Введите номер формы (1 или 2): ");
            if (scanf("%d%c", &num, &next) != 2 || (next != '\n' && next != ' '))
            {
                puts("Ошибка ввода.");
                clear_input();
                continue;
            }
            if (next == ' ') clear_input();

            int idx = num - 1;
            if (idx < 0 || idx > 1)
            {
                puts("Неверный номер формы");
                continue;
            }
            const LinearForm *lf = forms[idx];
            void *scalar = malloc(lf->typeInfo->size);
            if (!scalar)
            {
                puts("Ошибка выделения памяти.");
                continue;
            }
            printf("Введите скаляр: ");
            lf->typeInfo->scan(scalar);

            LinearForm *res = createLinearForm(lf->typeInfo, NULL, lf->n, &operationResult);
            if (operationResult != LINEARFORM_OPERATION_OK)
            {
                puts("Ошибка создания результирующей формы");
                free(scalar);
                continue;
            }
            LinearFormErrors err = multiplyScalarLinearForm(lf, scalar, res);
            if (err != LINEARFORM_OPERATION_OK)
            {
                printf("Ошибка операции: %d\n", err);
                freeLinearForm(res);
            }
            else
            {
                printf("Результат: ");
                printLinearForm(res);
                freeLinearForm(res);
            }
            free(scalar);
        }
        else if (choice == 4)
        {
            int num;
            printf("Введите номер формы (1 или 2): ");
            if (scanf("%d%c", &num, &next) != 2 || (next != '\n' && next != ' '))
            {
                puts("Ошибка ввода.");
                clear_input();
                continue;
            }
            if (next == ' ') clear_input();

            int idx = num - 1;
            if (idx < 0 || idx > 1)
            {
                puts("Неверный номер формы");
                continue;
            }
            const LinearForm *lf = forms[idx];
            void *args = malloc(lf->n * lf->typeInfo->size);
            if (!args)
            {
                puts("Ошибка выделения памяти.");
                continue;
            }
            for (uint32_t i = 0; i < lf->n; ++i)
            {
                printf("Введите значение x%u: ", i + 1);
                lf->typeInfo->scan((char *)args + i * lf->typeInfo->size);
            }
            void *result = malloc(lf->typeInfo->size);
            if (!result)
            {
                puts("Ошибка выделения памяти.");
                free(args);
                continue;
            }
            LinearFormErrors err = evaluateLinearForm(lf, args, result);
            if (err != LINEARFORM_OPERATION_OK)
            {
                printf("Ошибка вычисления: %d\n", err);
            }
            else
            {
                printf("Значение формы: ");
                lf->typeInfo->print(result);
                putchar('\n');
            }
            free(args);
            free(result);
        }
        else
        {
            puts("Неверный выбор");
        }
    } while (choice != 0);

    for (int i = 0; i < 2; ++i)
    {
        freeLinearForm(forms[i]);
    }
}