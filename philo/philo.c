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

static void	start_philo_threads(t_data *d)
{
	t_philolist	*cur;

	cur = d->philos;
	while (cur)
	{
		pthread_create(&cur->philo.thread, NULL,
			philo_routine, &cur->philo);
		cur = cur->next;
	}
}

static void	join_philo_threads(t_data *d)
{
	t_philolist	*cur;

	cur = d->philos;
	while (cur)
	{
		pthread_join(cur->philo.thread, NULL);
		cur = cur->next;
	}
}

int	main(int ac, char **av)
{
	t_data		*d;
	pthread_t	monitor;

	if (ac != 5 && ac != 6)
		return (1);
	d = init_data(av, ac);
	init_mutexes(d);
	init_philos(d);
	start_philo_threads(d);
	pthread_create(&monitor, NULL, monitor_routine, d);
	pthread_join(monitor, NULL);
	join_philo_threads(d);
	return (0);
}
