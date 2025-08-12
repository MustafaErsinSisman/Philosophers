/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musisman <musisman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 12:23:57 by musisman          #+#    #+#             */
/*   Updated: 2025/08/12 17:01:12 by musisman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_data	*init_data(char **av, int ac)
{
	t_data	*d;

	d = malloc(sizeof(t_data));
	if (!d)
		return (NULL);
	d->philos = NULL;
	d->num_of_philos = atoi(av[1]);
	d->time_to_die = atoi(av[2]);
	d->time_to_eat = atoi(av[3]);
	d->time_to_sleep = atoi(av[4]);
	if (ac == 6)
		d->meals_required = atoi(av[5]);
	else
		d->meals_required = -1;
	d->start_time = current_time_ms();
	d->someone_died = 0;
	d->forks = malloc(sizeof(pthread_mutex_t) * d->num_of_philos);
	return (d);
}

t_philolist	*create_node(int id, t_data *d)
{
	t_philolist	*node;

	node = malloc(sizeof(t_philolist));
	if (!node)
		return (NULL);
	node->philo.id = id;
	node->philo.meals_eaten = 0;
	node->philo.last_meal_time = d->start_time;
	node->philo.left_fork = &d->forks[id - 1];
	node->philo.right_fork = &d->forks[id % d->num_of_philos];
	node->philo.data = d;
	node->next = NULL;
	return (node);
}

void	init_philos(t_data *d)
{
	int			i;
	t_philolist	*prev;
	t_philolist	*node;

	d->philos = NULL;
	prev = NULL;
	i = 1;
	while (i <= d->num_of_philos)
	{
		node = create_node(i, d);
		if (!d->philos)
			d->philos = node;
		if (prev)
			prev->next = node;
		prev = node;
		i++;
	}
}
