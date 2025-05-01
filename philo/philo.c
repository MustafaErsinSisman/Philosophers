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

// ----------------------------------------
// mail sayısı için değişkenler ve methodlar


// int mails = 0;
// pthread_mutex_t mutex;

// void *routine()
// {
//         int i;

//         i  = 0;
//         while (i++ <  10100100)
//         {
//                 pthread_mutex_lock(&mutex);
//                 mails++;
//                 pthread_mutex_unlock(&mutex);
//         }
// }
// ----------------------------------------

// ----------------------------------------
// zar  atma için değişkenler ve methdolar


// void *roll_dice()
// {
//         int value = (rand() % 6) + 1;
//         int *res =  malloc(sizeof(int));
//         *res = value;
//         // printf("%d\n", value);
//         return (void *) res;
// }
// ----------------------------------------

// ----------------------------------------
// parametre gönderme değişkenleri ve methodları | 2 thread ile


// int primes[10] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};

// void *routine(void *arg)
// {
//         // sleep(1);   // whaaat?
//         int index = *(int *)arg;
//         int sum= 0;
//         int i = -1;
//         while (++i < 5)
//         {
//                 sum += primes[index + i];
//         }
//         // printf("%d ", primes[index]);
//         *(int *)arg =  sum;
//         return (arg);
// }
// ----------------------------------------

// ----------------------------------------
// trylock ve lock arasındaki farkların değişkenler ive methodları

// pthread_mutex_t mutex;

// void *routine(void *arg)
// {
//         // pthread_mutex_lock(&mutex);
//         if (!pthread_mutex_trylock(&mutex))
//         {
//                 printf("Got lock\n");
//                 sleep(1);
//                 pthread_mutex_unlock(&mutex);
//         }
//         else
//         {
//                 printf("Didn't get lock\n");
//         }

// }
// ----------------------------------------

// ----------------------------------------
//yakıt doldurma değişkenleri ve methodları


pthread_mutex_t mutexFuel;
pthread_cond_t condFuel;
int fuel = 0;

void *fuel_filling(void *arg)
{
        int i;

        i = -1;
        while (++i < 5)
        {
                pthread_mutex_lock(&mutexFuel);
                fuel += 15;
                printf("Filled fuel... %d\n", fuel);
                pthread_mutex_unlock(&mutexFuel);
                pthread_cond_signal(&condFuel);
                sleep(1);
        }
}

void *car(void *arg)
{
        pthread_mutex_lock(&mutexFuel);
        while (fuel < 40)
        {
                printf("No enough fuel\n");
                pthread_cond_wait(&condFuel,  &mutexFuel);
        }
        
        fuel -= 40;
        printf("Got fuel. Now left: %d\n", fuel);
        pthread_mutex_unlock(&mutexFuel);

}
// ----------------------------------------

int main(int ac, char **av)
{
// ----------------------------------------
// yakıt doldurma örneği


        pthread_t th[2];
        pthread_mutex_init(&mutexFuel, NULL);
        pthread_cond_init(&condFuel, NULL);
        int i;

        i = -1;
        while (++i < 2)
        {
                if (i == 1)
                {
                        if (pthread_create(&th[i], NULL, &fuel_filling, NULL))
                                perror("Failed to create thread");
                }
                else
                {
                        if (pthread_create(&th[i], NULL, &car, NULL))
                                perror("Faled to create thread");
                }
        }
        i = -1;
        while (++i < 2)
        {
                if (pthread_join(th[i], NULL))
                        perror("Failet to join thread");
        }
        pthread_mutex_destroy(&mutexFuel);
        pthread_cond_destroy(&condFuel);
// ----------------------------------------

// ----------------------------------------
// trylock ve lock arasındaki fark


        // pthread_t th[4];
        // pthread_mutex_init(&mutex, NULL);
        // int i;

        // i = -1;
        // while (++i < 4)
        // {
        //         if (pthread_create(&th[i], NULL, &routine, NULL))
        //                 perror("Error at creating thread");
        // }
        // i = -1;
        // while (++i < 4)
        // {
        //         if (pthread_join(th[i], NULL))
        //                 perror("Error at joining thread");
        // }
        // pthread_mutex_destroy(&mutex);
// ----------------------------------------

// ----------------------------------------
// parametre gönderme değişkenleri ve methodları | 2 thread ile


        // pthread_t th[2];
        // int i;
        // int *a;

        // i = -1;
        // while (++i < 2)
        // {
        //         a = malloc(sizeof(int));
        //         *a = i * 5;
        //         if (pthread_create(&th[i], NULL, &routine, a))
        //                 perror("Failed to create thread");
        // }
        // i = -1;
        // int  global_sum = 0;
        // while (++i <2)
        // {
        //         int *res;
        //         if (pthread_join(th[i], (void **)&res))
        //                 perror("Failed to join thread");
        //         global_sum += *res;
        //         free(res);
        // }
        // printf("Global sum: %d\n", global_sum);
// ----------------------------------------


// ----------------------------------------
// zar atma örneği | join için referans ile ve method için return değeri ile


        // int *res;

        // srand(time(NULL));
        // pthread_t th;

        // if (pthread_create(&th, NULL, &roll_dice,  NULL))
        //         return 1;
        // if (pthread_join(th, (void **)&res))
        //         return 2;
        // printf("Result: %d\n", *res);
        // free(res);
// ----------------------------------------

// ----------------------------------------
// mail sayısı örneği


        // int i;
        // pthread_t th[4];
        // pthread_mutex_init(&mutex, NULL);

        // i = -1;
        // while (++i < 4)
        // {
        //         if (pthread_create(&th[i], NULL, &routine, NULL))
        //         {
        //                 perror("Failed to create thread");
        //                 return 1;
        //         }
        //         printf("Thread %d has started\n", i);
        // }
        // i = -1;
        // while (++i < 4)
        // {
        //         if (pthread_join(th[i],  NULL))
        //         {
        //                 perror("Failed to finish thread");
        //                 return 2;
        //         }
        //         printf("Thread %d has finished execution\n", i);
        // }
        
        // pthread_mutex_destroy(&mutex);
        // printf("mails = %d\n", mails);

// ----------------------------------------
        
// ----------------------------------------
//filo başlangıç :D

        // if (ac == 5 || ac == 6)
        // {

        // }
        // else
        //         error(ERR_ARG);
// ----------------------------------------

        return 0;
}
