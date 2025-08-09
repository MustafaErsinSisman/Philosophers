#include "philo.h"

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	
	// Tek filozof durumu - sadece bekleyip ölecek
	if (philo->data->philo_count == 1)
	{
		print_status(philo, FORK_MSG);
		smart_sleep(philo->data->time_to_die + 1);
		return (NULL);
	}
	
	if (philo->id % 2 == 0)
		smart_sleep(philo->data->time_to_eat / 2);
	while (1)
	{
		pthread_mutex_lock(&philo->data->death_mutex);
		if (philo->data->someone_died)
		{
			pthread_mutex_unlock(&philo->data->death_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->data->death_mutex);
		philo_eat(philo);
		philo_sleep(philo);
		philo_think(philo);
	}
	return (NULL);
}

void	philo_eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->death_mutex);
	if (philo->data->someone_died)
	{
		pthread_mutex_unlock(&philo->data->death_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->data->death_mutex);
	
	// Tek filozof özel durumu
	if (philo->data->philo_count == 1)
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, FORK_MSG);
		pthread_mutex_unlock(philo->left_fork);
		return ;
	}
	
	// Deadlock prevention: çift id'ler farklı sırada alır
	if (philo->id % 2 == 1)
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, FORK_MSG);
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, FORK_MSG);
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, FORK_MSG);
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, FORK_MSG);
	}
	
	pthread_mutex_lock(&philo->data->meal_mutex);
	philo->last_meal_time = get_time();
	pthread_mutex_unlock(&philo->data->meal_mutex);
	
	print_status(philo, EAT_MSG);
	smart_sleep(philo->data->time_to_eat);
	
	pthread_mutex_lock(&philo->data->meal_mutex);
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->data->meal_mutex);
	
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	philo_sleep(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->death_mutex);
	if (philo->data->someone_died)
	{
		pthread_mutex_unlock(&philo->data->death_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->data->death_mutex);
	
	print_status(philo, SLEEP_MSG);
	smart_sleep(philo->data->time_to_sleep);
}

void	philo_think(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->death_mutex);
	if (philo->data->someone_died)
	{
		pthread_mutex_unlock(&philo->data->death_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->data->death_mutex);
	
	print_status(philo, THINK_MSG);
}