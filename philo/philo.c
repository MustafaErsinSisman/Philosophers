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

void	parse_args(t_data *data, char **av)
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

int	main(int ac, char **av)
{
        t_data *data = malloc(sizeof(t_data));
	if (ac != 5 && ac != 6)
		return (error(ERR_ARG));
	if (check_arg(av))
		return (error(ERR_ARG));
        parse_args(data, av);
	return (0);
}
