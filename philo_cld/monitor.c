#include "philo.h"

void	*monitor_routine(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (1)
	{
		// Önce yemek sayısını kontrol et - daha önemli
		if (check_meals(data))
			break ;
		// Sonra ölüm kontrolü
		if (check_death(data))
			break ;
		usleep(50);
	}
	return (NULL);
}

int	check_death(t_data *data)
{
	int			i;
	long long	current_time;

	i = 0;
	current_time = get_time();
	while (i < data->philo_count)
	{
		pthread_mutex_lock(&data->meal_mutex);
		if (current_time - data->philos[i].last_meal_time > data->time_to_die)
		{
			pthread_mutex_unlock(&data->meal_mutex);
			pthread_mutex_lock(&data->death_mutex);
			data->someone_died = 1;
			pthread_mutex_unlock(&data->death_mutex);
			pthread_mutex_lock(&data->print_mutex);
			current_time = get_time() - data->start_time;
			printf("%lld %d %s\n", current_time, data->philos[i].id, DIED_MSG);
			pthread_mutex_unlock(&data->print_mutex);
			return (1);
		}
		pthread_mutex_unlock(&data->meal_mutex);
		i++;
	}
	return (0);
}

int	check_meals(t_data *data)
{
	int	i;
	int	all_finished;

	if (data->must_eat_count == -1)
		return (0);
	
	all_finished = 1;
	i = 0;
	while (i < data->philo_count)
	{
		pthread_mutex_lock(&data->meal_mutex);
		if (data->philos[i].meals_eaten < data->must_eat_count)
			all_finished = 0;
		pthread_mutex_unlock(&data->meal_mutex);
		if (!all_finished)
			break ;
		i++;
	}
	
	if (all_finished)
	{
		pthread_mutex_lock(&data->death_mutex);
		data->someone_died = 1;
		pthread_mutex_unlock(&data->death_mutex);
		return (1);
	}
	return (0);
}