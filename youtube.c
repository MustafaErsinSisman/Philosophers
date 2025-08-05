/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   youtube.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musisman <musisman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:41:26 by musisman          #+#    #+#             */
/*   Updated: 2025/08/05 19:09:31 by musisman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo/philo.h"

void	*routine()
{
	printf("Test threads\n");
        return (0);
}

int	main(int ac, char **av)
{
	pthread_t	t1;

        (void)ac;
        (void)av;
	pthread_create(&t1, NULL, &routine, NULL);
	pthread_join(t1, NULL);
	return (0);
}
