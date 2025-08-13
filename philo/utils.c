/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musisman <<musisman@student.42.fr>>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:41:28 by musisman          #+#    #+#             */
/*   Updated: 2025/03/25 14:41:28 by musisman         ###   ########.fr       */
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
	long long	start;

	start = current_time_ms();
	while (current_time_ms() - start < ms)
	{
		pthread_mutex_lock(&d->write_mutex);
		if (d->someone_died)
		{
			pthread_mutex_unlock(&d->write_mutex);
			break ;
		}
		pthread_mutex_unlock(&d->write_mutex);
		usleep(100);
	}
}

void	print_state(t_philo *p, char *state)
{
	t_data	*d;

	d = p->data;
	pthread_mutex_lock(&d->write_mutex);
	if (!d->someone_died)
		printf("%lld %d %s\n",
			current_time_ms() - d->start_time, p->id, state);
	pthread_mutex_unlock(&d->write_mutex);
}
