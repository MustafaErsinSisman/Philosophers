#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <string.h>

# define FORK_MSG "has taken a fork"
# define EAT_MSG "is eating"
# define SLEEP_MSG "is sleeping"
# define THINK_MSG "is thinking"
# define DIED_MSG "died"

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long long		last_meal_time;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	struct s_data	*data;
}	t_philo;

typedef struct s_data
{
	int				philo_count;
	long long		time_to_die;
	long long		time_to_eat;
	long long		time_to_sleep;
	int				must_eat_count;
	int				finished_eating;
	int				someone_died;
	long long		start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	death_mutex;
	pthread_mutex_t	meal_mutex;
	t_philo			*philos;
}	t_data;

// utils.c
long long	get_time(void);
void		smart_sleep(long long time);
void		print_status(t_philo *philo, char *status);
int			ft_atoi(const char *str);

// init.c
int			init_data(t_data *data, int argc, char **argv);
int			init_philos(t_data *data);
int			init_mutexes(t_data *data);

// philo.c
void		*philosopher_routine(void *arg);
void		philo_eat(t_philo *philo);
void		philo_sleep(t_philo *philo);
void		philo_think(t_philo *philo);

// monitor.c
void		*monitor_routine(void *arg);
int			check_death(t_data *data);
int			check_meals(t_data *data);

// main.c
int			main(int argc, char **argv);
void		cleanup(t_data *data);
int			start_simulation(t_data *data);

#endif