#include "philosophers.h"

int	check_zero(char *num)
{
	int i;

	i = 0;
	while(num[i] == '0')
		i++;
	if(num[i] == '\0')
		return (1);
	return (0);
}

int	ft_isdigit(int ch)
{
	if (ch >= '0' && ch <= '9')
		return (1);
	return (0);
}

int is_valid(char *num)
{
	int i;

	i = 0;
	if(!num[i])
		return (0);
	while(num[i])
	{
		if(!ft_isdigit(num[i]))
			return (0);
		i++;
	}
	return (1);
}

int	init_print_and_sim(t_table *table)
{
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

long long	get_time()
{
	struct timeval tv;
	long long ms;

	gettimeofday(&tv, NULL);
	ms = (long long)tv.tv_sec * 1000 + tv.tv_usec / 1000;
	return (ms);
}

int	fill_table(t_table *table, char **av, int ac, long long ms)
{
	//table = malloc(sizeof(t_table));
	table->philo_num = ft_atoi(av[1]);
	table->time_to_die = ft_atoi(av[2]);
	table->time_to_eat = ft_atoi(av[3]);
	table->time_to_sleep = ft_atoi(av[4]);
	table->num_of_meals = -1;
	if(ac == 6)
		table->num_of_meals = ft_atoi(av[5]);
	if(init_forks(table, table->philo_num) == 0)
		return (0);
	if(init_print_and_sim(table) == 0)
		return (0);
	table->start_time = (ms - get_time());
	return (1);
}

t_philo *assign_forks(t_table *table)
{
	int i;
	t_philo *philo;

	i = 1;
	philo = malloc(sizeof(t_philo) * table->philo_num);
	philo[0].left_fork = &table->forks[(table->philo_num - 1)];
	philo[0].right_fork = &table->forks[0];
	while(i < table->philo_num)
	{
		philo[i].table = table;
		philo[i - 1].left_fork = &table->forks[i - 1];
		philo[i].right_fork = &table->forks[i];
		i++;
	}
	return philo;
}

void *routine(void *arg)
{
	printf("allo %s\n", (char *)arg);
	return NULL;
}

void create_threads(t_philo *philo, t_table *table)
{
	int i;

	i = 0;
	while(i < table->philo_num)
	{
		philo[i]->id = i;
		pthread_create(&philo[i].thread, NULL, routine, "allo");
		i++;
	}
	i = 0;
	while(i < table->philo_num)
	{
		pthread_join(philo[i].thread, NULL);
		i++;
	}
}

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
				printf("invalid number\n");
				return (1);
			}
		}
		fill_table(&table, av, ac, ms);
		philo = assign_forks(&table);
		create_threads(philo, &table);
	}
	else 
		printf("agiuwheawiugawiugdawiudgawuh\n");
}
