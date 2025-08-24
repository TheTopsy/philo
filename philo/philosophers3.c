#include "philosophers.h"

int	eat(t_table *table, t_philo *philo)
{
	if ((take_forks(table, philo)) == 0)
		return (0);
	if ((print_act("is eating", table, philo)) == 0)
		return (0);
	pthread_mutex_lock(&philo->status_mutex);
	if (philo->meals_eaten == table->num_of_meals)
		table->philos_full++;
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->status_mutex);
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal_time = get_time() - table->start_time;
	pthread_mutex_unlock(&philo->meal_mutex);
	if (table->time_to_eat < table->time_to_die)
		usleep(table->time_to_eat * 1000);
	else
		usleep(table->time_to_die * 1000);
	return (1);
}

int	print_sleep(t_table *table, t_philo *philo)
{
	if ((print_act("is sleeping", table, philo)) == 0)
		return (0);
	if (table->time_to_sleep < table->time_to_die)
	{
		usleep(table->time_to_sleep * 1000);
	}
	else
		usleep(table->time_to_die * 1000);
	return (1);
}

void	unlock_forks(t_philo *philo)
{
	if ((philo->id % 2) == 0)
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

int	check_death(t_table *table)
{
	pthread_mutex_lock(&table->sim_mutex);
	if (table->sim_end == 1)
	{
		pthread_mutex_unlock(&table->sim_mutex);
		return (0);
	}
	pthread_mutex_unlock(&table->sim_mutex);
	return (1);
}

void	sync_even(t_philo *philo, t_table *table)
{
	if ((philo->id % 2) == 0)
	{
		usleep(2000);
		print_think(philo, table);
	}
}