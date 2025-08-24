/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahab <adahab@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 13:00:10 by adahab            #+#    #+#             */
/*   Updated: 2025/08/24 13:00:30 by adahab           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void destroy_mutexes(t_table *table, t_philo *philo)
{
	int i;

	(void)philo;
	i = 0;
	pthread_mutex_destroy(&table->creation_mutex);
	pthread_mutex_destroy(&table->sim_mutex);
	pthread_mutex_destroy(&table->time_mutex);
	pthread_mutex_destroy(&table->print_mutex);
	while(i < table->philo_num)
	{
		pthread_mutex_destroy(&table->forks[i]);
		//free(table->philo);
		i++;
	}
	free(table->forks);
	free(table->philo[0]);
	free(table->philo);
	//pthread_mutex_destroy(&philo->meal_mutex);
	//pthread_mutex_destroy(&philo->status_mutex);
	//free(table);
}
int	main(int ac, char **av)
{
	int i;
	t_table table;
	t_philo *philo;
	long long ms;

	ms = get_time();
	i = 1;
	if (ac == 5 || ac == 6)
	{
		while (av[i])
		{
			if (!check_zero(av[i]) && is_valid(av[i]))
				i++;
			else
			{
				printf("Error : invalid number\n");
				return (1);
			}
		}
		fill_table(&table, av, ac);
		if (check_zero_v2(&table) == 0)
		{
			printf("Error : invalid number");
			return (1);
		}
		if (table.philo_num == 1)
		{
			one_philo(ms, &table);
			return (0);
		}
		philo = assign_forks(&table);
		create_threads(philo, &table);
		destroy_mutexes(&table, philo);
	}
	else
		printf("Error : incorrect argument count (try 4 or 5 arguments)\n");
}
