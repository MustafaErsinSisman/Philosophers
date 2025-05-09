/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philolist.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musisman <musisman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 12:11:27 by musisman          #+#    #+#             */
/*   Updated: 2025/05/09 14:08:47 by musisman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*dst;

	dst = malloc(count * size);
	if (!dst)
		return (NULL);
        memset(dst, 0, count * size);
	return (dst);
}

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

