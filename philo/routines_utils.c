/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musisman <musisman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 17:05:38 by musisman          #+#    #+#             */
/*   Updated: 2025/08/16 15:54:29 by musisman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	one_philo_case(t_philo *p, t_data *d)
{
	pthread_mutex_lock(p->left_fork);
	print_state(p, "has taken a fork");
	smart_sleep(d->time_to_die, d);
	pthread_mutex_unlock(p->left_fork);
}

void	philo_eat(t_philo *p, t_data *d, pthread_mutex_t *first,
		pthread_mutex_t *second)
{
	take_forks(p, &first, &second, d->meals_required);
	pthread_mutex_lock(first);
	print_state(p, "has taken a fork");
	pthread_mutex_lock(second);
	print_state(p, "has taken a fork");
	pthread_mutex_lock(&d->write_mutex);
	p->last_meal_time = current_time_ms();
	if (!d->someone_died)
		printf("%lld %d is eating\n", current_time_ms() - d->start_time, p->id);
	pthread_mutex_unlock(&d->write_mutex);
	smart_sleep(d->time_to_eat, d);
	pthread_mutex_lock(&d->write_mutex);
	p->meals_eaten++;
	pthread_mutex_unlock(&d->write_mutex);
	pthread_mutex_unlock(second);
	pthread_mutex_unlock(first);
}

void	philo_sleep_and_think(t_philo *p, t_data *d)
{
	print_state(p, "is sleeping");
	smart_sleep(d->time_to_sleep, d);
	print_state(p, "is thinking");
	usleep(100);
}

int	check_death(t_data *d, t_philolist *cur)
{
	long long	now;

	now = current_time_ms();
	if (!d->someone_died
		&& now - cur->philo.last_meal_time > d->time_to_die)
	{
		d->someone_died = 1;
		printf("%lld %d died\n",
			now - d->start_time, cur->philo.id);
		pthread_mutex_unlock(&d->write_mutex);
		return (1);
	}
	return (0);
}

int	check_meals_done(t_data *d, t_philolist *cur)
{
	if (d->meals_required == -1
		|| cur->philo.meals_eaten < d->meals_required)
		return (0);
	return (1);
}
