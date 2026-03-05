#include "LinearForm.h"

int main()
{
    puts("Введите кол-во линейных форм:");
    int nubmer_of_LinearForms = 0;
    scanf("%d", &nubmer_of_LinearForms);
    if (nubmer_of_LinearForms <= 0)
    {
        puts("Число должно быть больше 0");
        return 1;
    }
    LinearForm **LinearForms = malloc(sizeof(LinearForm *) * nubmer_of_LinearForms);
    for (uint32_t i = 0; i < nubmer_of_LinearForms; i++)
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
        printf("Введите кол-во коэффицентов для %u-ой линейной формы", i);
        uint32_t number_of_coeffs = 0;
        scanf("%u", &number_of_coeffs);
        for (uint32_t j = 0; j < number_of_coeffs; j++)
        {
            printf("Введите %u коэффицент для %u-ой линейной формы", j, i);
            scanf("%d")
        }
    }
    LinearFormErrors operationResult = LINEARFORM_OPERATION_OK;
    return 0;
}
