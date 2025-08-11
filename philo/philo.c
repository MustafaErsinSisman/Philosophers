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

long long	current_time_ms(void)
{
	struct timeval	tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000LL + tv.tv_usec / 1000);
}

void	smart_sleep(long long ms, t_data *d)
{
	long long	start = current_time_ms();
	while (current_time_ms() - start < ms)
	{
		pthread_mutex_lock(&d->write_mutex);
		if (d->someone_died)
		{
			pthread_mutex_unlock(&d->write_mutex);
			break;
		}
		pthread_mutex_unlock(&d->write_mutex);
		usleep(100);
	}
}

void	print_state(t_philo *p, char *state)
{
	t_data	*d = p->data;

	pthread_mutex_lock(&d->write_mutex);
	if (!d->someone_died)
		printf("%lld %d %s\n",
			current_time_ms() - d->start_time, p->id, state);
	pthread_mutex_unlock(&d->write_mutex);
}

void	*philo_routine(void *arg)
{
	t_philo	*p = (t_philo *)arg;
	t_data	*d = p->data;

	/* single philosopher special-case */
	if (d->num_of_philos == 1)
	{
	    pthread_mutex_lock(p->left_fork);
	    print_state(p, "has taken a fork");
	    /* can't take second fork -> will die after time_to_die (or monitor will detect) */
	    smart_sleep(d->time_to_die, d);
	    pthread_mutex_unlock(p->left_fork);
	    return NULL;
	}
	if (p->id % 2 == 0)
		usleep(100);
	while (1)
	{
		/* stop condition check */
		pthread_mutex_lock(&d->write_mutex);
		if (d->someone_died
			|| (d->meals_required != -1
				&& p->meals_eaten >= d->meals_required))
		{
			pthread_mutex_unlock(&d->write_mutex);
			break;
		}
		pthread_mutex_unlock(&d->write_mutex);

		print_state(p, "is thinking");
		pthread_mutex_lock(p->left_fork);
		print_state(p, "has taken a fork");
		pthread_mutex_lock(p->right_fork);
		print_state(p, "has taken a fork");

		/* eating: atomic update + print */
		pthread_mutex_lock(&d->write_mutex);
		p->last_meal_time = current_time_ms();
		if (!d->someone_died)
			printf("%lld %d is eating\n",
				current_time_ms() - d->start_time, p->id);
		pthread_mutex_unlock(&d->write_mutex);

		smart_sleep(d->time_to_eat, d);

		pthread_mutex_lock(&d->write_mutex);
		p->meals_eaten++;
		pthread_mutex_unlock(&d->write_mutex);

		pthread_mutex_unlock(p->left_fork);
		pthread_mutex_unlock(p->right_fork);

		print_state(p, "is sleeping");
		smart_sleep(d->time_to_sleep, d);
	}
	return (NULL);
}

void	*monitor_routine(void *arg)
{
	t_data		*d = (t_data *)arg;
	t_philolist	*cur;
	long long	now;
	int			all_done;

	while (1)
	{
		all_done = 1;
		cur = d->philos;
		while (cur)
		{
			pthread_mutex_lock(&d->write_mutex);
			now = current_time_ms();
			if (!d->someone_died
				&& now - cur->philo.last_meal_time > d->time_to_die)
			{
				d->someone_died = 1;
				printf("%lld %d died\n",
					now - d->start_time, cur->philo.id);
				pthread_mutex_unlock(&d->write_mutex);
				return (NULL);
			}
			if (d->meals_required == -1
				|| cur->philo.meals_eaten < d->meals_required)
				all_done = 0;
			pthread_mutex_unlock(&d->write_mutex);
			cur = cur->next;
		}
		if (all_done)
			return (NULL);
		usleep(1000);
	}
}

t_philolist	*create_node(int id, t_data *d)
{
	t_philolist	*node = malloc(sizeof(t_philolist));

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

int	main(int ac, char **av)
{
	t_data		d;
	int			i;
	t_philolist	*cur;
	pthread_t	monitor;

	if (ac != 5 && ac != 6)
		return (1);
	d.num_of_philos = atoi(av[1]);
	d.time_to_die = atoi(av[2]);
	d.time_to_eat = atoi(av[3]);
	d.time_to_sleep = atoi(av[4]);
	d.meals_required = (ac == 6) ? atoi(av[5]) : -1;
	d.start_time = current_time_ms();
	d.someone_died = 0;
	d.forks = malloc(sizeof(pthread_mutex_t) * d.num_of_philos);
	pthread_mutex_init(&d.write_mutex, NULL);
	i = -1;
	while (++i < d.num_of_philos)
		pthread_mutex_init(&d.forks[i], NULL);
	init_philos(&d);
	cur = d.philos;
	while (cur)
	{
		pthread_create(&cur->philo.thread, NULL, philo_routine, &cur->philo);
		cur = cur->next;
	}
	pthread_create(&monitor, NULL, monitor_routine, &d);
	pthread_join(monitor, NULL);
	cur = d.philos;
	while (cur)
	{
		pthread_join(cur->philo.thread, NULL);
		cur = cur->next;
	}
	return (0);
}
