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
		if(num[i] == '\\')
			return(0);
		if(!ft_isdigit(num[i]))
			return (0);
		i++;
	}
	return (1);
}
//DESTROY PREVIOUS MUTEXTES WHEN AN INIT FAILS
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

long long	get_time()
{
	struct timeval tv;
	long long ms;

	gettimeofday(&tv, NULL);
	ms = (long long)tv.tv_sec * 1000 + tv.tv_usec / 1000;
	return (ms);
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

int print_act(char *act, t_table *table, t_philo *philo)
{
	pthread_mutex_lock(&table->print_mutex);
	pthread_mutex_lock(&table->sim_mutex);
	if(table->sim_end == 1)
	{
		pthread_mutex_unlock(&table->sim_mutex);
		pthread_mutex_unlock(&table->print_mutex);
		return (0);
	}
	printf("%lld %d %s\n", get_time() - table->start_time, philo->id, act);
	pthread_mutex_unlock(&table->sim_mutex);
	pthread_mutex_unlock(&table->print_mutex);
	return (1);
}

int ft_usleep(long time_to_sleep, t_table *table)
{
    long curr;

    curr = 0;
    while(curr < time_to_sleep)
    {
        pthread_mutex_lock(&table->sim_mutex);
		if(table->sim_end == 1)
        {
            pthread_mutex_unlock(&table->sim_mutex);
            return (0);
        }
        pthread_mutex_unlock(&table->sim_mutex);
    	usleep(100);
    	curr += 100;        
    }
    return (1);
}

int print_think(t_philo *philo, t_table *table)
{
	if ((print_act("is thinking", table, philo)) == 0)
		return 0;
	if (table->philo_num % 2 && table->time_to_sleep <= table->time_to_eat)
	{
		ft_usleep(table->time_to_eat , table);
	}
	return 1;
}

int take_even(t_table *table, t_philo *philo)
{
	pthread_mutex_lock(philo->right_fork);
	if((print_act("has taken a fork", table, philo)) == 0)
	{
		pthread_mutex_unlock(philo->right_fork);
		return (0);
	}
	pthread_mutex_lock(philo->left_fork);
	if((print_act("has taken a fork", table, philo)) == 0)
	{
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
		return (0);
	}
	return (1);
}

int take_forks(t_table *table, t_philo *philo)
{
	if((philo->id % 2) == 0)
	{
		if(take_even(table, philo) == 0)
			return (0);
	}
	else
	{
		usleep(800);
		pthread_mutex_lock(philo->left_fork);
		if((print_act("has taken a fork", table, philo)) == 0)
		{
			pthread_mutex_unlock(philo->left_fork);
			return (0);
		}
		pthread_mutex_lock(philo->right_fork);
		if((print_act("has taken a fork", table, philo)) == 0)
		{
			pthread_mutex_unlock(philo->left_fork);
			pthread_mutex_unlock(philo->right_fork);
			return (0);
		}
	}
	return (1);
}

int eat(t_table *table, t_philo *philo)
{
	if((take_forks(table, philo)) == 0)
		return (0);
	if((print_act("is eating", table, philo)) == 0)
		return (0);
	pthread_mutex_lock(&philo->status_mutex);
	if(philo->meals_eaten == table->num_of_meals)
		table->philos_full++;
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->status_mutex);
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal_time = get_time() - table->start_time;
	pthread_mutex_unlock(&philo->meal_mutex);
	if(table->time_to_eat < table->time_to_die)
		usleep(table->time_to_eat * 1000);
	else
		usleep(table->time_to_die * 1000);
	return (1);
}

int print_sleep(t_table *table, t_philo *philo)
{
	if((print_act("is sleeping", table, philo)) == 0)
		return (0);
	if(table->time_to_sleep < table->time_to_die)
	{
		usleep(table->time_to_sleep * 1000);
	}
	else
		usleep(table->time_to_die * 1000);
    return (1);
}

void fill_start_time(t_table *table)
{
	pthread_mutex_lock(&table->time_mutex);
	table->start_time = get_time();
	pthread_mutex_unlock(&table->time_mutex);
}

void unlock_forks(t_philo *philo)
{
	if((philo->id % 2) == 0)
	{
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
	}
	else
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
	}
}
int check_death(t_table *table)
{
	pthread_mutex_lock(&table->sim_mutex);
	if(table->sim_end == 1)
	{
		pthread_mutex_unlock(&table->sim_mutex);
		return (0);
	}
	pthread_mutex_unlock(&table->sim_mutex);
	return (1);
}

void sync_even(t_philo *philo, t_table *table)
{
	if((philo->id % 2) == 0)
	{
		usleep(2000);
		print_think(philo, table);
	}
}

void *routine(void *arg)
{
	t_table *table;
	t_philo *philo;

	philo = (t_philo *)arg;
	table = philo->table;
	pthread_mutex_lock(&table->creation_mutex);	
	pthread_mutex_unlock(&table->creation_mutex);
	fill_start_time(table);
	sync_even(philo, table);
	while(1)
	{
		if(check_death(table) == 0)
			break;
		if(eat(table, philo) == 0)
		{
			unlock_forks(philo);
			break;
		}
		unlock_forks(philo);
		if(print_sleep(table, philo) == 0)
			break;
		if(print_think(philo, table) == 0)
			break;
	}
	return NULL;
}

int is_philo_full(t_table *table, t_philo *philo)
{
	pthread_mutex_lock(&philo->status_mutex);
		pthread_mutex_lock(&table->sim_mutex);
		if(table->philos_full == table->philo_num)
		{
			table->sim_end = 1;
			pthread_mutex_unlock(&table->sim_mutex);
			pthread_mutex_unlock(&philo->status_mutex);
			return (0);
	}
	pthread_mutex_unlock(&table->sim_mutex);
	pthread_mutex_unlock(&philo->status_mutex);
	return (1);
}
//philos communicate with each other ?

void print_death(t_table *table, t_philo *philo)
{
		pthread_mutex_lock(&table->print_mutex);
		pthread_mutex_lock(&table->time_mutex);
    	printf("%lld %d died\n", get_time() - table->start_time, philo->id);
		pthread_mutex_unlock(&table->time_mutex);
		pthread_mutex_unlock(&table->print_mutex);
}
void get_cur_time(long long *cur_time, t_table *table)
{	
	pthread_mutex_lock(&table->time_mutex);
	*cur_time = get_time() - table->start_time;
	pthread_mutex_unlock(&table->time_mutex);
}
void set_sim_end(t_table *table, t_philo *philo)
{
	pthread_mutex_lock(&table->sim_mutex);
	table->sim_end = 1;
	pthread_mutex_unlock(&table->sim_mutex);
	pthread_mutex_unlock(&philo->meal_mutex);
	print_death(table, philo);
}
void *monitor(void *arg)
{
	t_philo **philo;
	t_table *table;
	long long cur_time;
	int i;

	table = (t_table *)arg;
	philo = table->philo;
	i = 0;
	while(1)
	{
		if(i == table->philo_num)
			i = 0;
		if(is_philo_full(table, philo[i]) == 0)
			break;
		pthread_mutex_lock(&philo[i]->meal_mutex);
		get_cur_time(&cur_time, table);
		if((cur_time - philo[i]->last_meal_time) >= table->time_to_die && philo[i]->last_meal_time != 0)
		{
			set_sim_end(table, philo[i]);
			break;
		}
		pthread_mutex_unlock(&philo[i]->meal_mutex);
		i++;
	}
	return (NULL);
}

void create_threads(t_philo *philo, t_table *table)
{
	int i;

	i = 0;
	table->start_time = get_time();
	pthread_mutex_lock(&table->creation_mutex);
	while(i < table->philo_num)
	{
		philo[i].last_meal_time = 0;
		philo[i].id = (i + 1);
		philo[i].table = table;
		pthread_create(&philo[i].thread, NULL, routine, &philo[i]);
		i++;
	}
	pthread_mutex_unlock(&table->creation_mutex);
	pthread_create(&table->supervisor, NULL, monitor, table);
	i = 0;
	while(i < table->philo_num)
	{
		pthread_join(philo[i].thread, NULL);
		i++;
	}
	pthread_join(table->supervisor,NULL);
}

void *handle_one_philo(void *arg)
{
	t_table *table;

	table = (t_table *)arg;
	printf("%lld 1 has taken a fork\n", get_time() - table->start_time);
	usleep((table->time_to_die * 1000));
	printf("%lld 1 died\n", (get_time() - table->start_time));
	return NULL;
}

void one_philo(long long ms, t_table *table)
{
	t_philo *philo;

	philo = malloc(sizeof(t_philo));
	table->start_time = ms;
	pthread_create(&philo->thread, NULL, handle_one_philo, table);
	pthread_join(philo->thread, NULL);
}

int	check_zero_v2(t_table *table)
{
	if(table->philo_num == 0)
		return (0);
	if(table->time_to_die == 0)
		return (0);
	if(table->time_to_eat == 0)
		return (0);
	if(table->time_to_sleep == 0)
		return (0);
	if(table->num_of_meals == 0)
		return (0);
	return (1);
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

