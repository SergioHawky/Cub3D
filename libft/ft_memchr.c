/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 12:22:45 by seilkiv           #+#    #+#             */
/*   Updated: 2024/11/11 16:13:16 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// This function will search for the char c in that block of memory
// It return a pointer to the first ocurrence of c in the block
// If c is not in the block it will return NULL

void	*ft_memchr(const void *s, int c, size_t n)
{
	const unsigned char	*p;

	p = (unsigned char *)s;
	while (n > 0)
	{
		if (*p == (unsigned char)c)
		{
			return ((void *)p);
		}
		p++;
		n--;
	}
	return (NULL);
}

/*#include <stdio.h>
int	main(void)
{
	char c[11] = "test string";

	printf("%s\n", (char *)ft_memchr(c, 's', 5));
	return (0);
}*/