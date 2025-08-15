/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musisman <musisman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 17:05:38 by musisman          #+#    #+#             */
/*   Updated: 2025/08/15 20:37:06 by musisman         ###   ########.fr       */
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
	take_forks(p, &first, &second);
	pthread_mutex_lock(first);
	print_state(p, "has taken a fork");
	pthread_mutex_lock(second);
	print_state(p, "has taken a fork");
	pthread_mutex_lock(&d->state_mutex);
	p->last_meal_time = current_time_ms();
	if (!d->someone_died)
	{
		pthread_mutex_lock(&d->write_mutex);
		printf("%lld %d is eating\n", current_time_ms() - d->start_time, p->id);
		pthread_mutex_unlock(&d->write_mutex);
	}
	pthread_mutex_unlock(&d->state_mutex);
	smart_sleep(d->time_to_eat, d);
	pthread_mutex_lock(&d->state_mutex);
	p->meals_eaten++;
	pthread_mutex_unlock(&d->state_mutex);
	pthread_mutex_unlock(second);
	pthread_mutex_unlock(first);
}

void	philo_sleep(t_philo *p, t_data *d)
{
	print_state(p, "is sleeping");
	smart_sleep(d->time_to_sleep, d);
}

int	check_death(t_data *d, t_philolist *cur)
{
	long long	now;

	now = current_time_ms();
	if (!d->someone_died && now - cur->philo.last_meal_time > d->time_to_die)
	{
		d->someone_died = 1;
		pthread_mutex_lock(&d->write_mutex);
		printf("%lld %d died\n", now - d->start_time, cur->philo.id);
		pthread_mutex_unlock(&d->write_mutex);
		return (1);
	}
	return (0);
}

int	check_meals_done(t_data *d, t_philolist *cur)
{
	(void)d;
	if (cur->philo.meals_eaten >= 0 && d->meals_required != -1
		&& cur->philo.meals_eaten >= d->meals_required)
		return (1);
	return (0);
}
