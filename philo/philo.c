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

// int	main(int ac, char **av)
// {
// 	t_data	data;

// 	if ((ac != 5 && ac != 6) || check_arg(av))
// 		return (error(ERR_ARG));
// 	if (inits(&data, av))
// 		return (error(ERR_INIT));
// 	return (0);
// }


/* ---------- helpers ---------- */
long long current_time_ms(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000LL + tv.tv_usec / 1000LL);
}

void print_state(t_philo *p, const char *msg)
{
	t_data *d = p->data;
	pthread_mutex_lock(&d->write_mutex);
	if (!d->someone_died)
		printf("%lld %d %s\n", current_time_ms() - d->start_time, p->id, msg);
	pthread_mutex_unlock(&d->write_mutex);
}

void smart_sleep(long long ms, t_data *d)
{
	long long start = current_time_ms();
	while (!d->someone_died && current_time_ms() - start < ms)
		usleep(500); // küçük uyku; death check için
}

/* ---------- philosopher routine ---------- */
void *philo_routine(void *arg)
{
	t_philo *p = arg;
	t_data *d = p->data;

	/* Single philosopher special-case */
	if (d->num_of_philos == 1)
	{
		pthread_mutex_lock(p->left_fork);
		print_state(p, "has taken a fork");
		/* can't take second fork -> will die after time_to_die */
		smart_sleep(d->time_to_die, d);
		pthread_mutex_unlock(p->left_fork);
		return NULL;
	}

	/* stagger start to reduce contention */
	if (p->id % 2 == 0)
		usleep(100);

	while (!d->someone_died
		&& (d->meals_required == -1 || p->meals_eaten < d->meals_required))
	{
		/* avoid circular wait: even philosophers pick right then left */
		if (p->id % 2 == 0)
		{
			pthread_mutex_lock(p->right_fork);
			print_state(p, "has taken a fork");
			pthread_mutex_lock(p->left_fork);
			print_state(p, "has taken a fork");
		}
		else
		{
			pthread_mutex_lock(p->left_fork);
			print_state(p, "has taken a fork");
			pthread_mutex_lock(p->right_fork);
			print_state(p, "has taken a fork");
		}

		/* start eating: update last_meal_time safely */
		pthread_mutex_lock(&d->write_mutex);
		p->last_meal_time = current_time_ms();
		pthread_mutex_unlock(&d->write_mutex);

		print_state(p, "is eating");
		smart_sleep(d->time_to_eat, d);
		p->meals_eaten++;

		pthread_mutex_unlock(p->left_fork);
		pthread_mutex_unlock(p->right_fork);

		if (d->meals_required != -1 && p->meals_eaten >= d->meals_required)
			break;

		print_state(p, "is sleeping");
		smart_sleep(d->time_to_sleep, d);

		print_state(p, "is thinking");
	}
	return NULL;
}

/* ---------- monitor thread ---------- */
void *monitor_routine(void *arg)
{
	t_data *d = arg;
	t_philolist *cur;

	while (!d->someone_died)
	{
		cur = d->philos;
		while (cur)
		{
			pthread_mutex_lock(&d->write_mutex);
			long long since = current_time_ms() - cur->philo.last_meal_time;
			if (since > d->time_to_die)
			{
				d->someone_died = 1;
				printf("%lld %d died\n", current_time_ms() - d->start_time, cur->philo.id);
				pthread_mutex_unlock(&d->write_mutex);
				return NULL;
			}
			pthread_mutex_unlock(&d->write_mutex);
			cur = cur->next;
		}
		if (d->meals_required != -1)
		{
			int all_full = 1;
			cur = d->philos;
			while (cur)
			{
				if (cur->philo.meals_eaten < d->meals_required)
				{
					all_full = 0;
					break;
				}
				cur = cur->next;
			}
			if (all_full)
			{
				pthread_mutex_lock(&d->write_mutex);
				d->someone_died = 1; // use as stop flag
				pthread_mutex_unlock(&d->write_mutex);
				return NULL;
			}
		}
		usleep(1000);
	}
	return NULL;
}

/* ---------- init / cleanup / main ---------- */
int init_mutexes(t_data *d)
{
	int i;
	d->forks = malloc(sizeof(pthread_mutex_t) * d->num_of_philos);
	if (!d->forks) return -1;
	for (i = 0; i < d->num_of_philos; i++)
		pthread_mutex_init(&d->forks[i], NULL);
	pthread_mutex_init(&d->write_mutex, NULL);
	return 0;
}

t_philolist *init_philos(t_data *d)
{
	int i;
	t_philolist *head = NULL, *prev = NULL, *node;

	for (i = 0; i < d->num_of_philos; i++)
	{
		node = malloc(sizeof(*node));
		if (!node) return NULL;
		node->philo.id = i + 1;
		node->philo.meals_eaten = 0;
		node->philo.last_meal_time = d->start_time;
		node->philo.left_fork = &d->forks[i];
		node->philo.right_fork = &d->forks[(i + 1) % d->num_of_philos];
		node->philo.data = d;
		node->next = NULL;
		if (!head) head = node;
		if (prev) prev->next = node;
		prev = node;
	}
	d->philos = head;
	return head;
}

void cleanup(t_data *d)
{
	t_philolist *cur = d->philos;
	t_philolist *tmp;
	int i;

	/* destroy mutexes */
	for (i = 0; i < d->num_of_philos; i++)
		pthread_mutex_destroy(&d->forks[i]);
	pthread_mutex_destroy(&d->write_mutex);

	/* free forks array */
	free(d->forks);

	/* free nodes */
	while (cur)
	{
		tmp = cur->next;
		free(cur);
		cur = tmp;
	}
}

/* simple arg parsing */
int parse_args(int argc, char **argv, t_data *d)
{
	if (argc < 5 || argc > 6) return -1;
	d->num_of_philos = atoi(argv[1]);
	d->time_to_die = atoi(argv[2]);
	d->time_to_eat = atoi(argv[3]);
	d->time_to_sleep = atoi(argv[4]);
	d->meals_required = (argc == 6) ? atoi(argv[5]) : -1;
	if (d->num_of_philos <= 0 || d->time_to_die <= 0 || d->time_to_eat <= 0 || d->time_to_sleep <= 0)
		return -1;
	d->start_time = current_time_ms();
	d->someone_died = 0;
	d->philos = NULL;
	d->forks = NULL;
	return 0;
}

int main(int argc, char **argv)
{
	t_data data;
	t_philolist *cur;
	pthread_t monitor;

	if (parse_args(argc, argv, &data) == -1)
	{
		fprintf(stderr, "Usage: %s num time_to_die time_to_eat time_to_sleep [meals]\n", argv[0]);
		return 1;
	}
	if (init_mutexes(&data) == -1) return 1;
	if (!init_philos(&data)) return 1;

	/* create philosopher threads */
	cur = data.philos;
	while (cur)
	{
		if (pthread_create(&cur->philo.thread, NULL, philo_routine, &cur->philo) != 0)
		{
			perror("pthread_create");
			return 1;
		}
		cur = cur->next;
	}

	/* start monitor */
	if (pthread_create(&monitor, NULL, monitor_routine, &data) != 0)
	{
		perror("pthread_create monitor");
		return 1;
	}

	/* wait monitor */
	pthread_join(monitor, NULL);

	/* join philosopher threads */
	cur = data.philos;
	while (cur)
	{
		pthread_join(cur->philo.thread, NULL);
		cur = cur->next;
	}

	cleanup(&data);
	return 0;
}

