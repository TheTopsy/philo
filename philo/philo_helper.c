#include "philosophers.h"

int     ft_atoi(const char *str)
{
        int     flag;
        int     i;
        int     result;

        flag = 1;
        i = 0;
        result = 0;
        while (str[i] && ((str[i] >= 9 && str[i] <= 13) || str[i] == ' '))
                i++;
        if (str[i] == '-' || str[i] == '+')
        {
                if (str[i] == '-')
                        flag = flag * (-1);
                i++;
        }
        while (str[i] && str[i] >= '0' && str[i] <= '9')
        {
                result = (result * 10) + (str[i] - 48);
                i++;
        }
        return (result * flag);
}
