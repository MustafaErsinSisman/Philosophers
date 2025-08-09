/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   youtube.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:41:26 by musisman          #+#    #+#             */
/*   Updated: 2025/08/09 17:44:30 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "deneme.h"


// mutex

// int mails = 0;
// pthread_mutex_t mutex;

// void	*routine()
// {
// 	size_t count = 10101010;
	
// 	for (size_t i = 0; i < count; i++)
// 	{
// 		pthread_mutex_lock(&mutex);
// 		mails++;
// 		pthread_mutex_unlock(&mutex);
// 	}
	
// 	return (0);
// }

#include <time.h>

void *roll_dice()
{
	int value = (rand() % 6) + 1;
	int *res = malloc(sizeof(int));
	
	*res = value;
	printf("value: %d\n", value);
	return (void  *)res;
}

int	main(int ac, char **av)
{
    (void)ac;
    (void)av;
	int *res, *res2;
	srand(time(NULL));
	pthread_t th, th2;
	if (pthread_create(&th, NULL, &roll_dice, NULL))
		return 1;
	if (pthread_create(&th2, NULL, &roll_dice, NULL))
		return 1;
	if (pthread_join(th, (void  **)&res))
		return 2;
	if (pthread_join(th2, (void  **)&res2))
		return 2;
	printf("res: %d, res2: %d\n", *res, *res2);
	free(res);








	
	// mutex
	
	// pthread_t	th[8];
	// int i = 0;
	
	// pthread_mutex_init(&mutex, NULL);
	// while (i < 8)
	// {
	// 	if (pthread_create(&th[i++], NULL, &routine, NULL))
	// 	{
	// 		perror("Failed to creat thread\n");
	// 		return (1);
	// 	}
	// 	printf("thread %d: has started\n", i);
	// }
	// i = 0;
	// printf("\n");
	// while (i < 8)
	// {
	// 	if (pthread_join(th[i++], NULL))
	// 	{
	// 		perror("Failed to join thread\n");
	// 		return (2);
	// 	}
	// 	printf("thread %d: has joined\n", i);
	// }
	// pthread_mutex_destroy(&mutex);
	// printf("\nNumber of mails: %d\n", mails);
	
	return (0);
}
