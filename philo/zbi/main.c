#include "philosophers.h"

int main(int ac, char **av)
{
	int i;
	t_table table;
	t_philo *philo;
	long long ms;

	ms = get_time();
	i = 1;
	if(ac == 5 || ac == 6)
	{
		while(av[i])
		{
			if(!check_zero(av[i]) && is_valid(av[i]))
				i++;
			else
			{
				printf("Error : invalid number\n");
				return (1);
			}
		}
		fill_table(&table, av, ac);
		if(check_zero_v2(&table) == 0)
		{
			printf("Error : invalid number");
			return (1);
		}	
		if(table.philo_num == 1)
		{
			one_philo(ms, &table);
			return(0);
		}
		philo = assign_forks(&table);
		create_threads(philo, &table);
	}
	else 
		printf("Error : incorrect argument count (try 4 or 5 arguments)\n");
}