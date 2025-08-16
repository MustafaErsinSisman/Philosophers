#include "philo.h"

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc < 5 || argc > 6)
	{
		printf("Usage: ./philo nb_philo time_to_die time_to_eat time_to_sleep [nb_must_eat]\n");
		return (1);
	}
	if (!validate_args(argc, argv))
	{
		printf("Error: Invalid arguments\n");
		return (1);
	}
	memset(&data, 0, sizeof(t_data));
	if (!init_data(&data, argc, argv))
	{
		printf("Error: Initialization failed\n");
		return (1);
	}
	if (!init_philos(&data))
	{
		printf("Error: Philosopher initialization failed\n");
		cleanup(&data);
		return (1);
	}
	if (!start_simulation(&data))
	{
		printf("Error: Simulation failed\n");
		cleanup(&data);
		return (1);
	}
	cleanup(&data);
	return (0);
}

int	validate_args(int argc, char **argv)
{
	int	i;
	int	j;

	i = 1;
	while (i < argc)
	{
		j = 0;
		while (argv[i][j])
		{
			if (!ft_isdigit(argv[i][j]))
				return (0);
			j++;
		}
		if (ft_atoi(argv[i]) <= 0)
			return (0);
		i++;
	}
	return (1);
}

int	init_data(t_data *data, int argc, char **argv)
{
	int	i;

	data->nb_philo = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->nb_must_eat = ft_atoi(argv[5]);
	else
		data->nb_must_eat = -1;
	
	data->someone_died = 0;
	data->all_ate = 0;
	data->start_time = get_current_time();
	
	data->forks = malloc(sizeof(pthread_mutex_t) * data->nb_philo);
	if (!data->forks)
		return (0);
	
	i = 0;
	while (i < data->nb_philo)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
			return (0);
		i++;
	}
	
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&data->death_mutex, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&data->meal_mutex, NULL) != 0)
		return (0);
	
	return (1);
}

int	init_philos(t_data *data)
{
	int	i;

	data->philos = malloc(sizeof(t_philo) * data->nb_philo);
	if (!data->philos)
		return (0);
	
	i = 0;
	while (i < data->nb_philo)
	{
		data->philos[i].id = i + 1;
		data->philos[i].meals_eaten = 0;
		data->philos[i].last_meal_time = get_current_time();
		data->philos[i].data = data;
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[(i + 1) % data->nb_philo];
		i++;
	}
	return (1);
}