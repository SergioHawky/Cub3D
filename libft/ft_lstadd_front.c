/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 12:07:39 by seilkiv           #+#    #+#             */
/*   Updated: 2024/11/11 12:28:34 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (!lst || !new)
		return ;
	new->next = *lst;
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

 int main()
 {
		t_list *head = NULL;
		t_list *node1 = ft_lstnew("Segundo");
		t_list *node2 = ft_lstnew("Primeiro");

		ft_lstadd_front(&head, node1);
		ft_lstadd_front(&head, node2);

		printf("Lista após adicionar os nós:\n");
		print_list(head);
		free(node1);
		free(node2);

		return (0);
}*/