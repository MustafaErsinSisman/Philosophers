/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musisman <<musisman@student.42.fr>>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:41:37 by musisman          #+#    #+#             */
/*   Updated: 2025/03/25 14:41:37 by musisman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

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

typedef struct s_philolist
{
	t_philo				philo;
	struct s_philolist	*next;
}	t_philolist;

typedef struct s_data
{
	int				num_of_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				meals_required;
	long long		start_time;
	int				someone_died;
	pthread_mutex_t	*forks;
	int				*init_forks_bool;
	pthread_mutex_t	write_mutex;
	int				init_write_bool;
	pthread_mutex_t	state_mutex;
	int				init_state_bool;
	t_philolist		*philos;
}	t_data;

t_data		*init_data(char **av, int ac);
int			ft_atoi(const char *str);
int			init_data_mutexes(t_data *d);
int			init_philos(t_data *d);
long long	current_time_ms(void);
void		smart_sleep(long long ms, t_data *d);
void		print_state(t_philo *p, char *state);
void		take_forks(t_philo *p, pthread_mutex_t **first,
				pthread_mutex_t **second);
void		one_philo_case(t_philo *p, t_data *d);
void		philo_eat(t_philo *p, t_data *d, pthread_mutex_t *first,
				pthread_mutex_t *second);
void		philo_sleep(t_philo *p, t_data *d);
void		*philo_routine(void *arg);
int			check_death(t_data *d, t_philolist *cur);
int			check_meals_done(t_data *d, t_philolist *cur);
void		*monitor_routine(void *arg);

#endif
