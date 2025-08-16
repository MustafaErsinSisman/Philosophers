#ifndef PHILO_H
# define PHILO_H

# define _GNU_SOURCE
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <string.h>

typedef struct s_philo	t_philo;

typedef struct s_data
{
	int				nb_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nb_must_eat;
	int				someone_died;
	int				all_ate;
	long long		start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	death_mutex;
	pthread_mutex_t	meal_mutex;
	t_philo			*philos;
}	t_data;

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long long		last_meal_time;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_data			*data;
}	t_philo;

/* Main functions */
int			main(int argc, char **argv);
int			init_data(t_data *data, int argc, char **argv);
int			init_philos(t_data *data);
int			start_simulation(t_data *data);
void		*philosopher_routine(void *arg);
void		*monitor_routine(void *arg);

/* Utility functions */
int			ft_atoi(const char *str);
int			ft_isdigit(int c);
int			validate_args(int argc, char **argv);
long long	get_current_time(void);
void		ft_usleep(int milliseconds);
void		print_status(t_philo *philo, char *status);

/* Actions */
void		take_forks(t_philo *philo);
void		eat(t_philo *philo);
void		sleep_and_think(t_philo *philo);

/* Cleanup */
void		cleanup(t_data *data);
int			check_death(t_data *data);
int			check_all_ate(t_data *data);

#endif