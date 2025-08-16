#include "philo.h"

void	take_forks(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->death_mutex);
	if (philo->data->someone_died || philo->data->all_ate)
	{
		pthread_mutex_unlock(&philo->data->death_mutex);
		return;
	}
	pthread_mutex_unlock(&philo->data->death_mutex);
	
	if (philo->data->nb_philo == 1)
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
		usleep(philo->data->time_to_die * 1000);
		pthread_mutex_unlock(philo->left_fork);
		return;
	}
	
	if (philo->id % 2 == 1)
	{
		pthread_mutex_lock(philo->left_fork);
		pthread_mutex_lock(&philo->data->death_mutex);
		if (philo->data->someone_died || philo->data->all_ate)
		{
			pthread_mutex_unlock(&philo->data->death_mutex);
			pthread_mutex_unlock(philo->left_fork);
			return;
		}
		pthread_mutex_unlock(&philo->data->death_mutex);
		print_status(philo, "has taken a fork");
		
		pthread_mutex_lock(philo->right_fork);
		pthread_mutex_lock(&philo->data->death_mutex);
		if (philo->data->someone_died || philo->data->all_ate)
		{
			pthread_mutex_unlock(&philo->data->death_mutex);
			pthread_mutex_unlock(philo->left_fork);
			pthread_mutex_unlock(philo->right_fork);
			return;
		}
		pthread_mutex_unlock(&philo->data->death_mutex);
		print_status(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		pthread_mutex_lock(&philo->data->death_mutex);
		if (philo->data->someone_died || philo->data->all_ate)
		{
			pthread_mutex_unlock(&philo->data->death_mutex);
			pthread_mutex_unlock(philo->right_fork);
			return;
		}
		pthread_mutex_unlock(&philo->data->death_mutex);
		print_status(philo, "has taken a fork");
		
		pthread_mutex_lock(philo->left_fork);
		pthread_mutex_lock(&philo->data->death_mutex);
		if (philo->data->someone_died || philo->data->all_ate)
		{
			pthread_mutex_unlock(&philo->data->death_mutex);
			pthread_mutex_unlock(philo->left_fork);
			pthread_mutex_unlock(philo->right_fork);
			return;
		}
		pthread_mutex_unlock(&philo->data->death_mutex);
		print_status(philo, "has taken a fork");
	}
}

void	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->death_mutex);
	if (philo->data->someone_died || philo->data->all_ate)
	{
		pthread_mutex_unlock(&philo->data->death_mutex);
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		return;
	}
	pthread_mutex_unlock(&philo->data->death_mutex);
	
	print_status(philo, "is eating");
	
	pthread_mutex_lock(&philo->data->meal_mutex);
	philo->last_meal_time = get_current_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->data->meal_mutex);
	
	usleep(philo->data->time_to_eat * 1000);
	
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	sleep_and_think(t_philo *philo)
{
	print_status(philo, "is sleeping");
	usleep(philo->data->time_to_sleep * 1000);
	print_status(philo, "is thinking");
}

void	print_status(t_philo *philo, char *status)
{
	long long	timestamp;

	pthread_mutex_lock(&philo->data->print_mutex);
	
	pthread_mutex_lock(&philo->data->death_mutex);
	if (philo->data->someone_died || philo->data->all_ate)
	{
		if (strcmp(status, "died") != 0)
		{
			pthread_mutex_unlock(&philo->data->death_mutex);
			pthread_mutex_unlock(&philo->data->print_mutex);
			return;
		}
	}
	pthread_mutex_unlock(&philo->data->death_mutex);
	
	timestamp = get_current_time() - philo->data->start_time;
	printf("%lld %d %s\n", timestamp, philo->id, status);
	
	pthread_mutex_unlock(&philo->data->print_mutex);
}