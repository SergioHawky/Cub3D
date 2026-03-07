/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 12:11:11 by seilkiv           #+#    #+#             */
/*   Updated: 2024/11/11 12:20:21 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*node;

	node = (t_list *)malloc(sizeof(t_list));
	if (!node)
		return (NULL);
	node->content = content;
	node->next = NULL;
	return (node);
}
/*#include <stdio.h>
int	main(void) {
	char *content = "Hello, world!";
	t_list *node = ft_lstnew(content);

	if (node == NULL) {
		printf("Falha ao criar o nó.\n");
		return (1);
	}
	printf("Conteúdo do nó: %s\n", (char *)node->content);
	free(node);

	return (0);
}*/