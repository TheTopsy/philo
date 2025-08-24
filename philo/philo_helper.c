#include "philosophers.h"

long	ft_atoi(const char *str)
{
	int		flag;
	int		i;
	long	result;

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
		if (result >= INT_MAX)
			return (0);
		i++;
	}
	return (result * flag);
}

int	check_zero(char *num)
{
	int	i;

	i = 0;
	while (num[i] == '0')
		i++;
	if (num[i] == '\0')
		return (1);
	return (0);
}

int	ft_isdigit(int ch)
{
	if (ch >= '0' && ch <= '9')
		return (1);
	return (0);
}

int	is_valid(char *num)
{
	int	i;

	i = 0;
	if (!num[i])
		return (0);
	while (num[i])
	{
		if (num[i] == '\\')
			return (0);
		if (!ft_isdigit(num[i]))
			return (0);
		i++;
	}
	return (1);
}

long long	get_time(void)
{
	struct timeval	tv;
	long long		ms;

	gettimeofday(&tv, NULL);
	ms = (long long)tv.tv_sec * 1000 + tv.tv_usec / 1000;
	return (ms);
}
