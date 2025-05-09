/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musisman <musisman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 12:23:57 by musisman          #+#    #+#             */
/*   Updated: 2025/05/09 12:27:51 by musisman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int 	init_data(t_data *data, char **av)
{
        data->num_of_philos = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	if (av[5])
		data->meals_required = ft_atoi(av[5]);
	else
		data->meals_required = -1;
	data->start_time = 0;
	data->someone_died = 0;
	data->forks = NULL;
	// data->write_mutex = NULL;  //? neden gözükmüyor
	data->philos = NULL;
}

int	inits(t_data *data, char **av)
{
        if (init_datas(data, av))
                return (1);
}