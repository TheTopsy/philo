#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef pthread_mutex_t	t_mutex_t;

typedef struct s_table
{
	long				philo_num;
	long				time_to_die;
	long				time_to_eat;
	long				time_to_sleep;
	long				num_of_meals;
	long				philos_full;
	t_mutex_t			*forks;
	t_mutex_t			print_mutex;
	t_mutex_t			sim_mutex;
	t_mutex_t			creation_mutex;
	t_mutex_t			time_mutex;
	pthread_t			supervisor;
	char				sim_end;
	long long			start_time;
	struct s_philo		**philo;
}						t_table;

typedef struct s_philo
{
	int					id;
	pthread_t			thread;
	t_mutex_t			*left_fork;
	t_mutex_t			*right_fork;
	t_mutex_t			meal_mutex;
	t_mutex_t			status_mutex;
	long long			last_meal_time;
	int					meals_eaten;
	char				is_alive;
	struct s_table		*table;

}						t_philo;

long					ft_atoi(const char *str);
int						check_zero(char *num);
int						ft_isdigit(int ch);
int						is_valid(char *num);
int						init_print_and_sim(t_table *table);
int						init_forks(t_table *table, int forks_num);
long long				get_time(void);
int						fill_table(t_table *table, char **av, int ac);
t_philo					*assign_forks(t_table *table);
int						print_act(char *act, t_table *table, t_philo *philo);
int						ft_usleep(long time_to_sleep, t_table *table);
int						print_think(t_philo *philo, t_table *table);
int						take_even(t_table *table, t_philo *philo);
int						take_forks(t_table *table, t_philo *philo);
int						eat(t_table *table, t_philo *philo);
int						print_sleep(t_table *table, t_philo *philo);
void					fill_start_time(t_table *table);
void					unlock_forks(t_philo *philo);
int						check_death(t_table *table);
void					sync_even(t_philo *philo, t_table *table);
void					*routine(void *arg);
int						is_philo_full(t_table *table, t_philo *philo);
void					print_death(t_table *table, t_philo *philo);
void					get_cur_time(long long *cur_time, t_table *table);
void					set_sim_end(t_table *table, t_philo *philo);
void					*monitor(void *arg);
void					create_threads(t_philo *philo, t_table *table);
void					*handle_one_philo(void *arg);
void					one_philo(long long ms, t_table *table);
int						check_zero_v2(t_table *table);
void destroy_mutexes(t_table *table);
#endif
