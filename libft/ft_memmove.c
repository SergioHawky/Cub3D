/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 14:08:15 by seilkiv           #+#    #+#             */
/*   Updated: 2024/11/05 16:07:57 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// This functions copies n bytes from src into dest
// It takes care of overlaping memory
// It returns a pointer to n the start of dest

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*d;
	unsigned char	*s;

	d = dest;
	s = (unsigned char *)src;
	if (!dest && !src)
		return (dest);
	if (d < s)
	{
		while (n--)
		{
			*d++ = *s++;
		}
	}
	else
	{
		d += n;
		s += n;
		while (n--)
		{
			*(--d) = *(--s);
		}
	}
	return (dest);
}

/*#include <stdio.h>
#include <string.h>

int	main(void) {
	char src1[] = "Hello, World!";
	char dest1[20];

	printf("Antes: src = \"%s\", dest = \"%s\"\n", src1, dest1);
	ft_memmove(dest1, src1, strlen(src1) + 1);
	printf("Depois: src = \"%s\", dest = \"%s\"\n\n", src1, dest1);

	return (0);
}*/