/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musisman <<musisman@student.42.fr>>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:41:26 by musisman          #+#    #+#             */
/*   Updated: 2025/03/25 14:41:26 by musisman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	free_all(t_data *d)
{
	t_philolist	*cur;
	t_philolist	*tmp;
	int			i;
	int			error_flag;

	error_flag = 0;
	i = 0;
	while (i < d->num_of_philos)
	{
		if (d->init_forks_bool[i] && pthread_mutex_destroy(&d->forks[i]))
			error_flag = 1;
		i++;
	}
	if (d->init_write_bool && pthread_mutex_destroy(&d->write_mutex))
		error_flag = 1;
	cur = d->philos;
	while (cur)
	{
		tmp = cur->next;
		free(cur);
		cur = tmp;
	}
	free(d->forks);
	free(d);
	return (error_flag);
}

static int	start_philo_threads(t_data *d)
{
	t_philolist	*cur;
	t_philolist	*tmp;

	cur = d->philos;
	while (cur)
	{
		if (pthread_create(&cur->philo.thread, NULL,
				philo_routine, &cur->philo))
		{
			tmp = d->philos;
			while (tmp != cur)
			{
				pthread_join(tmp->philo.thread, NULL);
				tmp = tmp->next;
			}
			return (1);
		}
		cur = cur->next;
	}
	return (0);
}

static int	monitor_thread(t_data *d)
{
	pthread_t	monitor;

	if (pthread_create(&monitor, NULL, monitor_routine, d))
		return (1);
	if (pthread_join(monitor, NULL))
		return (1);
	return (0);
}

static int	join_philo_threads(t_data *d)
{
	t_philolist	*cur;
	int			error_flag;

	error_flag = 0;
	cur = d->philos;
	while (cur)
	{
		if (cur->philo.thread)
		{
			if (pthread_join(cur->philo.thread, NULL))
				error_flag = 1;
		}
		cur = cur->next;
	}
	return (error_flag);
}

int	main(int ac, char **av)
{
	t_data	*d;
	int		status;

	if (ac != 5 && ac != 6)
		return (1);
	d = init_data(av, ac);
	if (!d)
		return (1);
	if (init_data_mutexes(d) || init_philos(d))
		return (free_all(d));
	if (start_philo_threads(d) || monitor_thread(d)
		|| join_philo_threads(d))
		status = 1;
	else
		status = 0;
	if (free_all(d))
		status = 1;
	return (status);
}
