/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 12:25:38 by seilkiv           #+#    #+#             */
/*   Updated: 2024/11/11 12:26:34 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*node;

	if (!lst || !del)
		return ;
	while (*lst)
	{
		node = (*lst)->next;
		ft_lstdelone(*lst, del);
		*lst = node;
	}
}
/*#include <stdio.h>

void	del(void *content)
{
	free(content);
}

int	main(void)
{
	t_list *node1 = ft_lstnew(malloc(sizeof(int)));
	t_list *node2 = ft_lstnew(malloc(sizeof(int)));
	t_list *node3 = ft_lstnew(malloc(sizeof(int)));

	if (!node1 || !node2 || !node3)
	{
		printf("Erro ao alocar nós.\n");
		return (1);
	}

	node1->next = node2;
	node2->next = node3;

	printf("Tamanho da lista antes de limpar: %d\n", ft_lstsize(node1));
	ft_lstclear(&node1, del);
	if (node1 == NULL)
		printf("Lista limpa com sucesso.\n");
	else
		printf("Falha ao limpar a lista.\n");

	return (0);
}*/