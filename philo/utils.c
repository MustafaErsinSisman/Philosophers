/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musisman <<musisman@student.42.fr>>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:41:28 by musisman          #+#    #+#             */
/*   Updated: 2025/03/25 14:41:28 by musisman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	ft_putstr_fd(char *s, int fd)
{
	size_t	i;

	i = 0;
	while (s[i])
	{
		write(fd, &s[i], 1);
		i++;
	}
}

int	error(char *err)
{
	ft_putstr_fd(err, 2);
	return (-1);
}

int	ft_isdigit(int c)
{
	if ((c >= 48 && c <= 57) || (c == '-' || c == '+'))
		return (1);
	return (0);
}

int	ft_atoi(const char *str)
{
	size_t	i;
	size_t	nb;
	int		sing;

	i = 0;
	sing = 1;
	nb = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sing = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		nb = (str[i] - '0') + (nb * 10);
		i++;
	}
	return (nb * sing);
}

int	check_arg(char **str)
{
	int	i;
	int	j;

	i = 0;
	while (str[++i])
	{
		j = -1;
		while (str[i][++j])
		{		
			if (!ft_isdigit(str[i][j]))
				return (-1);	
		}
	}
	if (ft_atoi(str[1]) > 200)
		return (-1);
	i = 0;
	while (str[++i])
	{
		if (ft_atoi(str[i]) <= 0)
			return (-1);
	}
	return (0);
}
