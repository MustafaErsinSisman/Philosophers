/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   youtube.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:41:26 by musisman          #+#    #+#             */
/*   Updated: 2025/08/07 17:43:15 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "deneme.h"

int mails = 0;
pthread_mutex_t mutex;

void	*routine()
{
	size_t count = 10101010;
	
	for (size_t i = 0; i < count; i++)
	{
		pthread_mutex_lock(&mutex);
		mails++;
		pthread_mutex_unlock(&mutex);
	}
	
	return (0);
}

int	main(int ac, char **av)
{
    (void)ac;
    (void)av;

	pthread_t	th[8];
	int i = 0;
	
	pthread_mutex_init(&mutex, NULL);
	while (i < 8)
	{
		if (pthread_create(&th[i++], NULL, &routine, NULL))
		{
			perror("Failed to creat thread\n");
			return (1);
		}
		printf("thread %d: has started\n", i);
	}
	i = 0;
	printf("\n");
	while (i < 8)
	{
		if (pthread_join(th[i++], NULL))
		{
			perror("Failed to join thread\n");
			return (2);
		}
		printf("thread %d: has joined\n", i);
	}
	pthread_mutex_destroy(&mutex);
	printf("\nNumber of mails: %d\n", mails);
	return (0);
}
