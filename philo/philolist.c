/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philolist.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musisman <musisman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 12:11:27 by musisman          #+#    #+#             */
/*   Updated: 2025/05/09 12:15:06 by musisman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_philolist	*ft_lstnew(t_philo philo)
{
	t_philolist	*node;

	node = ft_calloc(1, sizeof(t_philo));
	if (!node)
		return (NULL);
	node->philo = philo;
	node->next = NULL;
	return (node);
}

void	ft_lstadd_front(t_philolist **lst, t_philolist *new)
{
	new->next = *lst;
	*lst = new;
}

t_philolist	*ft_lstlast(t_philolist *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	ft_lstadd_back(t_philolist **lst, t_philolist *new)
{
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	ft_lstlast(*lst)->next = new;
}
