/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musisman <musisman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 12:23:57 by musisman          #+#    #+#             */
/*   Updated: 2025/08/15 16:21:51 by musisman         ###   ########.fr       */
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
	d->num_of_philos = ft_atoi(av[1]);
	d->time_to_die = ft_atoi(av[2]);
	d->time_to_eat = ft_atoi(av[3]);
	d->time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
		d->meals_required = ft_atoi(av[5]);
	else
		d->meals_required = -1;
	d->start_time = current_time_ms();
	d->someone_died = 0;
	d->forks = malloc(sizeof(pthread_mutex_t) * d->num_of_philos);
	if (!d->forks)
	{
		free(d);
		return (NULL);
	}
	return (d);
}

int	init_data_mutexes(t_data *d)
{
	int	i;

	d->init_write_bool = 0;
	if (pthread_mutex_init(&d->write_mutex, NULL))
	{
		d->init_write_bool = 1;
		return (1);
	}
	i = -1;
	d->init_forks_bool = malloc(d->num_of_philos * sizeof(int));
	if (!d->init_forks_bool)
		return (1);
	while (++i < d->num_of_philos)
		d->init_forks_bool[i] = 0;
	i = -1;
	while (++i < d->num_of_philos)
	{
		if (pthread_mutex_init(&d->forks[i], NULL))
		{
			d->init_forks_bool[i] = 1;
			return (1);
		}
	}
	return (0);
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

static void	free_philo_nodes(t_philolist *head)
{
	t_philolist	*tmp;

	while (head)
	{
		tmp = head->next;
		free(head);
		head = tmp;
	}
}

int	init_philos(t_data *d)
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
		if (!node)
		{
			free_philo_nodes(d->philos);
			d->philos = NULL;
			return (1);
		}
		if (!d->philos)
			d->philos = node;
		if (prev)
			prev->next = node;
		prev = node;
		i++;
	}
	return (0);
}
