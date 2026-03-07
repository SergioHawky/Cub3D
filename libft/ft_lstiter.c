/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 12:10:23 by seilkiv           #+#    #+#             */
/*   Updated: 2024/11/11 12:22:42 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	t_list	*list_ptr;

	if (!lst)
		return ;
	list_ptr = lst;
	while (list_ptr != NULL)
	{
		(*f)(list_ptr->content);
		list_ptr = list_ptr->next;
	}
}
/*#include <stdio.h>

void	print_content(void *content)
{
	printf("Conteúdo: %d\n", *(int *)content);
}

int	main(void)
{
	t_list *node1 = ft_lstnew(malloc(sizeof(int)));
	t_list *node2 = ft_lstnew(malloc(sizeof(int)));

	if (!node1 || !node2 )
	{
		printf("Erro ao alocar memória para os nós.\n");
		return (1);
	}

	*(int *)node1->content = 10;
	*(int *)node2->content = 20;

	node1->next = node2;
	node2->next = NULL;

	ft_lstiter(node1, print_content);

	free(node1->content);
	free(node2->content);
	free(node1);
	free(node2);
	return (0);
}*/