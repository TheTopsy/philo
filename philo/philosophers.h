#ifndef PHILOSOPHERS_H
#define PHILOSOPHERS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>


typedef pthread_mutex_t t_mutex_t;

typedef struct s_table 
{
	int philo_num;
	int time_to_die;
	int time_to_eat;
	int time_to_sleep;
	int num_of_meals;
	int philos_full;
	t_mutex_t *forks;
	t_mutex_t print_mutex;
	t_mutex_t sim_mutex;
	t_mutex_t creation_mutex;
	t_mutex_t time_mutex;
	pthread_t supervisor;
	char sim_end;
	long long start_time;
	struct s_philo **philo;
} t_table;

typedef struct s_philo
{
	int id;
	pthread_t thread;
	t_mutex_t *left_fork;
	t_mutex_t *right_fork;
	t_mutex_t meal_mutex;
	t_mutex_t status_mutex;
	long long last_meal_time;
	int meals_eaten;
	char is_alive;
	struct s_table *table;
	
} t_philo;


int	ft_atoi(const char *str);

#endif
