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

void *routine()
{
        printf("test thrads\n");
        sleep(3);
        printf("end\n");
}

int main(int ac, char **av)
{
        if (ac == 5 || ac == 6)
        {
                pthread_t t1, t2;
                printf("%s\n", av[1]);
                if (pthread_create(&t1, NULL, &routine, NULL) != 0)
                        return (1);
                if (pthread_create(&t2, NULL, &routine, NULL) != 0)
                        return  (2);
                if (pthread_join(t1, NULL) != 0)
                        return(3);
                if (pthread_join(t2, NULL) != 0)
                        return(4);
        }
        else
                error(ERR_ARG);

        return 0;
}
