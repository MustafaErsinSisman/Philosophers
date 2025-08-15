/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musisman <musisman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 12:11:27 by musisman          #+#    #+#             */
/*   Updated: 2025/08/15 20:17:06 by musisman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philo_routine(void *arg)
{
	t_philo	*p;
	t_data	*d;

	p = (t_philo *)arg;
	d = p->data;
	if (d->num_of_philos == 1)
		return (one_philo_case(p, d), NULL);
	if (p->id % 2 == 0)
		usleep(100);
	while (1)
	{
		pthread_mutex_lock(&d->state_mutex);
		if (d->someone_died || (d->meals_required != -1
				&& p->meals_eaten >= d->meals_required))
		{
			pthread_mutex_unlock(&d->state_mutex);
			return (NULL);
		}
		pthread_mutex_unlock(&d->state_mutex);
		philo_eat(p, d, NULL, NULL);
		philo_sleep(p, d);
		print_state(p, "is thinking");
	}
	return (NULL);
}

void	*monitor_routine(void *arg)
{
	t_data		*d;
	t_philolist	*cur;
	int			all_done;

	d = (t_data *)arg;
	while (1)
	{
		all_done = 1;
		cur = d->philos;
		while (cur)
		{
			pthread_mutex_lock(&d->state_mutex);
			if (check_death(d, cur))
				return (pthread_mutex_unlock(&d->state_mutex), NULL);
			if (!check_meals_done(d, cur))
				all_done = 0;
			pthread_mutex_unlock(&d->state_mutex);
			cur = cur->next;
		}
		if (all_done)
			return (NULL);
		usleep(1000);
	}
}
