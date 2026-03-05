#include "LinearForm.h"

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
    LinearForm **LinearForms = malloc(sizeof(LinearForm *) * number_of_LinearForms);
    for (uint32_t i = 0; i < number_of_LinearForms; i++)
    {
        printf("Выберите тип элементов для %u-ой линейной формы: 1 - целые, 2 - вещественные числа с плавающей запятой, 3 - комплексные", i);
        uint32_t type_of_elements = 0;
        for (;;)
        {
            if ((scanf("%u", &type_of_elements)) != 1 || (type_of_elements != 1 && type_of_elements != 2 && type_of_elements != 3))
            {
                puts("Неверный тип, выберете целое число от 1 до 3");
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
        printf("Введите кол-во переменных для %u-ой линейной формы", i);
        uint32_t n = 0;
        scanf("%u", &n);
        uint32_t number_of_coeffs = n + 1;
        void *temp = malloc(number_of_coeffs * typeinfo->size);
        for (uint32_t j = 0; j < number_of_coeffs; j++)
        {
            printf("Введите %u коэффицент для %u-ой линейной формы", j, i);
            typeinfo->scan((char *)temp + j * typeinfo->size);
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
    for (uint32_t i = 0; i < number_of_LinearForms; i++)
    {
        freeLinearForm(LinearForms[i]);
    }
    free(LinearForms);

    return 0;
}