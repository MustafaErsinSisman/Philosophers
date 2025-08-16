#include "philo.h"

int	start_simulation(t_data *data)
{
	pthread_t	monitor_thread;
	int			i;

	i = 0;
	while (i < data->nb_philo)
	{
		if (pthread_create(&data->philos[i].thread, NULL, philosopher_routine, &data->philos[i]) != 0)
			return (0);
		i++;
	}
	
	if (pthread_create(&monitor_thread, NULL, monitor_routine, data) != 0)
		return (0);
	
	i = 0;
	while (i < data->nb_philo)
	{
		pthread_join(data->philos[i].thread, NULL);
		i++;
	}
	pthread_join(monitor_thread, NULL);
	
	return (1);
}

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	
	if (philo->id % 2 == 0)
		usleep(1000);
	
	while (1)
	{
		pthread_mutex_lock(&philo->data->death_mutex);
		if (philo->data->someone_died || philo->data->all_ate)
		{
			pthread_mutex_unlock(&philo->data->death_mutex);
			break;
		}
		pthread_mutex_unlock(&philo->data->death_mutex);
		
		take_forks(philo);
		eat(philo);
		sleep_and_think(philo);
	}
	return (NULL);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	
	while (1)
	{
		if (check_death(data) || check_all_ate(data))
			break;
		usleep(1000);
	}
	return (NULL);
}

int	check_death(t_data *data)
{
	int			i;
	long long	current_time;
	long long	time_since_meal;

	i = 0;
	while (i < data->nb_philo)
	{
		pthread_mutex_lock(&data->meal_mutex);
		current_time = get_current_time();
		time_since_meal = current_time - data->philos[i].last_meal_time;
		pthread_mutex_unlock(&data->meal_mutex);
		
		if (time_since_meal > data->time_to_die)
		{
			pthread_mutex_lock(&data->death_mutex);
			if (!data->someone_died && !data->all_ate)
			{
				data->someone_died = 1;
				pthread_mutex_unlock(&data->death_mutex);
				print_status(&data->philos[i], "died");
				return (1);
			}
			pthread_mutex_unlock(&data->death_mutex);
			return (1);
		}
		i++;
	}
	return (0);
}

int	check_all_ate(t_data *data)
{
	int	i;
	int	finished_eating;

	if (data->nb_must_eat == -1)
		return (0);
	
	pthread_mutex_lock(&data->death_mutex);
	if (data->someone_died)
	{
		pthread_mutex_unlock(&data->death_mutex);
		return (0);
	}
	pthread_mutex_unlock(&data->death_mutex);
	
	finished_eating = 1;
	i = 0;
	while (i < data->nb_philo)
	{
		pthread_mutex_lock(&data->meal_mutex);
		if (data->philos[i].meals_eaten < data->nb_must_eat)
			finished_eating = 0;
		pthread_mutex_unlock(&data->meal_mutex);
		if (!finished_eating)
			break;
		i++;
	}
	
	if (finished_eating)
	{
		pthread_mutex_lock(&data->death_mutex);
		if (!data->someone_died)
			data->all_ate = 1;
		pthread_mutex_unlock(&data->death_mutex);
		return (1);
	}
	return (0);
}