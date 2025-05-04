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

# define ERR_ARG "Argument error\n"

typedef struct s_info
{
	int philo_count;
	int time_to_die;
	int time_to_eat;
	int time_to_sleep;
	int must_eat_count;
	int someone_died;
	int start_time;
	pthread_mutex_t *fork;
	pthread_mutex_t print_lock;
}t_info;

typedef struct s_philo
{
	int id;
	int eat_count;
	long last_meal;
	pthread_t thread;
	pthread_mutex_t *left_fork;
	pthread_mutex_t *right_fork;
	t_info *info;
}t_philo;

int	error(char *err);
int	check_arg(char **str);

#endif