#include "philosophers.h"

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
