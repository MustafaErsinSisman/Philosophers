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

int	main(int ac, char **av)
{
	t_data		*d;
	int			i;
	t_philolist	*cur;
	pthread_t	monitor;

	if (ac != 5 && ac != 6)
		return (1);
	d = init_data(av, ac);
	pthread_mutex_init(&d->write_mutex, NULL);
	i = -1;
	while (++i < d->num_of_philos)
		pthread_mutex_init(&d->forks[i], NULL);
	init_philos(d);
	cur = d->philos;
	while (cur)
	{
		pthread_create(&cur->philo.thread, NULL, philo_routine, &cur->philo);
		cur = cur->next;
	}
	pthread_create(&monitor, NULL, monitor_routine, d);
	pthread_join(monitor, NULL);
	cur = d->philos;
	while (cur)
	{
		pthread_join(cur->philo.thread, NULL);
		cur = cur->next;
	}
	return (0);
}
