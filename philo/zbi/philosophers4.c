#include "philosophers.h"

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