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


void parse_args(t_info *info, char **av)
{
    info->philo_count = ft_atoi(av[1]);
    info->time_to_die = ft_atoi(av[2]);
    info->time_to_eat = ft_atoi(av[3]);
    info->time_to_sleep = ft_atoi(av[4]);
    if (av[5])
        info->must_eat_count = ft_atoi(av[5]);
    else
        info->must_eat_count = -1;
}

int	main(int ac, char **av)
{
	if (ac != 5 && ac != 6)
		return (error(ERR_ARG));
	if (check_arg(av) != 0)
		return (error(ERR_ARG));
        
	return (0);
}
