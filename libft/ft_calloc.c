/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:05:00 by seilkiv           #+#    #+#             */
/*   Updated: 2024/11/05 11:05:00 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*p;

	if (size != 0 && count > (size_t)-1 / size)
		return (NULL);
	p = malloc(count * size);
	if (!p)
		return (NULL);
	ft_bzero(p, count * size);
	return (p);
}

/*#include <stdio.h>
int	main(void)
{
	int n = 5;

	int *myAlloc = (int *)ft_calloc(n, sizeof(int));
	if (myAlloc == NULL)
	{
		printf("Memory allocation failed.\n");
		return (1);
	}
	for (int i = 0; i < n; i++)
	{
		printf("%d ", i);
		if (myAlloc[i] != 0)
		{
			printf("Error: Allocated memory is not initialized to zero.\n");
			return (1);
		}
	}
	free(myAlloc);
	return (0);
}*/