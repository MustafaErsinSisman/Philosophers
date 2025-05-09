/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musisman <musisman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 12:23:57 by musisman          #+#    #+#             */
/*   Updated: 2025/05/09 14:25:41 by musisman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int init_forks(t_data *data)
{
        int i;

        i = 0;
}

static int init_philolist(t_data *data)
{
        int i;
        t_philo philo;
        t_philolist *tmp;

        i = -1;
        while (++i < data->num_of_philos)
                ft_lstadd_back(&data->philos, ft_lstnew(philo));
        i = -1;
        while (++i < data->num_of_philos)
        {
                tmp = data->philos;
                tmp->philo.id = i;
                tmp->philo.meals_eaten = 0;
                tmp->philo.data = data;
                tmp = tmp->next; 
        }
        return (0);
}

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
        return (0);
}

int	inits(t_data *data, char **av)
{
        if (init_datas(data, av))
                return (1);
        if (init_philolist(data))
                return (1);
        if (init_forks(data))
                return (1);
        return (0);
}