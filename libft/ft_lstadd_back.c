/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 12:06:59 by seilkiv           #+#    #+#             */
/*   Updated: 2024/11/11 12:29:10 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	if (!lst || !new)
		return ;
	if (*lst)
		ft_lstlast(*lst)->next = new;
	else
		*lst = new;
}
/*#include <stdio.h>
void	print_list(t_list *lst)
{
		while (lst)
		{
			printf("%s -> ", (char *)lst->content);
			lst = lst->next;
		}
		printf("NULL\n");
}

int	main(void)
{
		t_list *list = NULL;  // Lista inicialmente vazia
		// Criar alguns nós com conteúdo
		t_list *node1 = ft_lstnew("Primeiro");
		t_list *node2 = ft_lstnew("Segundo");
		t_list *node3 = ft_lstnew("Terceiro");

		// Adicionar os nós à lista usando ft_lstadd_back
		ft_lstadd_back(&list, node1);
		ft_lstadd_back(&list, node2);
		ft_lstadd_back(&list, node3);

		// Imprimir a lista para verificar a ordem
		printf("Conteúdo da lista:\n");
		print_list(list);  // Deve imprimir"Primeiro->Segundo->Terceiro >NULL"

		return (0);
}*/