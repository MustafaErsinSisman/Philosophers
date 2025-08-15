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

int	ft_atoi(const char *str)
{
	size_t	i;
	size_t	nb;
	int		sing;

	i = 0;
	sing = 1;
	nb = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sing = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		nb = (str[i] - '0') + (nb * 10);
		i++;
	}
	return (nb * sing);
}

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

void	take_forks(t_philo *p, pthread_mutex_t **first,
			pthread_mutex_t **second, int meals_required)
{
	int	swap;

	swap = 0;
	if (meals_required % 2 == 0)
	{
		if (p->id % 2 != 0)
			swap = 1;
	}
	else if (p->id % 2 == 0)
		swap = 1;
	if (swap)
	{
		*first = p->right_fork;
		*second = p->left_fork;
	}
	else
	{
		*first = p->left_fork;
		*second = p->right_fork;
	}
}
