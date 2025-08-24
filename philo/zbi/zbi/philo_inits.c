#include "philosophers.h"

int	init_print_and_sim(t_table *table)
{
	if(pthread_mutex_init(&table->time_mutex, NULL) != 0)
		return (0);
	if(pthread_mutex_init(&table->creation_mutex, NULL) != 0)
		return (0);
	if(pthread_mutex_init(&table->sim_mutex, NULL) != 0)
		return (0);
	if(pthread_mutex_init(&table->print_mutex, NULL) != 0)
		return (0);
	return (1);
}

int	init_forks(t_table *table, int forks_num)
{
	int i;

	i = 0;
	if((table->forks = malloc(forks_num * sizeof(pthread_mutex_t))) == 0)
		return (0);
	while(i < forks_num)
	{
		if(pthread_mutex_init(&table->forks[i], NULL) != 0)
		{
			while(--i)
				pthread_mutex_destroy(&table->forks[i]);
			break;
		}
		i++;
	}
	return (1);
}

int	fill_table(t_table *table, char **av, int ac)
{
	//table = malloc(sizeof(t_table));
	table->philo_num = ft_atoi(av[1]);
	table->time_to_die = ft_atoi(av[2]);
	table->time_to_eat = ft_atoi(av[3]);
	table->time_to_sleep = ft_atoi(av[4]);
	table->sim_end = 0;
	table->philos_full = 0;
	table->num_of_meals = -1;
	if(ac == 6)
		table->num_of_meals = ft_atoi(av[5]);
	if(init_forks(table, table->philo_num) == 0)
		return (0);
	if(init_print_and_sim(table) == 0)
		return (0);
	//table->start_time = (get_time()); // 
	if((table->philo = malloc(table->philo_num * sizeof(t_table *))) == NULL)
		return (0);
	return (1);
}

t_philo *assign_forks(t_table *table)
{
	int i;
	t_philo *philo;

	i = 1;
	philo = malloc(sizeof(t_philo) * table->philo_num);
	philo[0].table = table;
	philo[0].meals_eaten = 0;
	table->philo[0] = &philo[0];
	pthread_mutex_init(&philo[0].meal_mutex, NULL);
	pthread_mutex_init(&philo[0].status_mutex, NULL);
	philo[0].left_fork = &table->forks[(table->philo_num - 1)];
	philo[0].right_fork = &table->forks[0];
	while(i < table->philo_num)
	{
		table->philo[i] = &philo[i];
		pthread_mutex_init(&philo[i].meal_mutex, NULL);
		pthread_mutex_init(&philo[i].status_mutex, NULL);
		philo[i].table = table;
		philo[i].meals_eaten = 0;
		philo[i].left_fork = &table->forks[i - 1];
		philo[i].right_fork = &table->forks[i];
		i++;
	}
	return philo;
}

void fill_start_time(t_table *table)
{
	pthread_mutex_lock(&table->time_mutex);
	table->start_time = get_time();
	pthread_mutex_unlock(&table->time_mutex);
}