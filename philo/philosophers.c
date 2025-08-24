/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahab <adahab@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 13:00:58 by adahab            #+#    #+#             */
/*   Updated: 2025/08/24 13:01:09 by adahab           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// DESTROY PREVIOUS MUTEXTES WHEN AN INIT FAILS
// philos communicate with each other ?
void	*monitor(void *arg)
{
	t_philo		**philo;
	t_table		*table;
	long long	cur_time;
	int			i;

	table = (t_table *)arg;
	philo = table->philo;
	i = 0;
	while (1)
	{
		if (i == table->philo_num)
			i = 0;
		if (is_philo_full(table, philo[i]) == 0)
			break ;
		pthread_mutex_lock(&philo[i]->meal_mutex);
		get_cur_time(&cur_time, table);
		if ((cur_time - philo[i]->last_meal_time) >= table->time_to_die
			&& philo[i]->last_meal_time != 0)
		{
			set_sim_end(table, philo[i]);
			break ;
		}
		pthread_mutex_unlock(&philo[i]->meal_mutex);
		i++;
	}
	return (NULL);
}

void	create_threads(t_philo *philo, t_table *table)
{
	int	i;

	i = 0;
	table->start_time = get_time();
	pthread_mutex_lock(&table->creation_mutex);
	while (i < table->philo_num)
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
	while (i < table->philo_num)
	{
		pthread_join(philo[i].thread, NULL);
		i++;
	}
	pthread_join(table->supervisor, NULL);
}

void	*handle_one_philo(void *arg)
{
	t_table	*table;

	table = (t_table *)arg;
	printf("%lld 1 has taken a fork\n", get_time() - table->start_time);
	usleep((table->time_to_die * 1000));
	printf("%lld 1 died\n", (get_time() - table->start_time));
	return (NULL);
}

void	one_philo(long long ms, t_table *table)
{
	t_philo	*philo;

	philo = malloc(sizeof(t_philo));
	table->start_time = ms;
	pthread_create(&philo->thread, NULL, handle_one_philo, table);
	pthread_join(philo->thread, NULL);
}

int	check_zero_v2(t_table *table)
{
	if (table->philo_num == 0)
		return (0);
	if (table->time_to_die == 0)
		return (0);
	if (table->time_to_eat == 0)
		return (0);
	if (table->time_to_sleep == 0)
		return (0);
	if (table->num_of_meals == 0)
		return (0);
	return (1);
}
