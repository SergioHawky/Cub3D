/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 14:54:43 by seilkiv           #+#    #+#             */
/*   Updated: 2024/11/07 12:50:07 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	unsigned int	d_len;
	unsigned int	s_len;
	size_t			i;
	size_t			j;

	s_len = ft_strlen(src);
	if (size == 0)
		return (size + s_len);
	d_len = ft_strlen(dst);
	i = d_len;
	j = 0;
	if (size <= d_len)
		return (size + s_len);
	while (src[j] && i < (size - 1))
	{
		dst[i] = src[j];
		i++;
		j++;
	}
	dst[i] = '\0';
	return (d_len + s_len);
}
/*#include <stdio.h>
int	main(void)
{
	char dst[50] = "Ola ";
	char *src = "Mundo !";

	printf("Meu: %zu\n", ft_strlcat(dst, src, 7));
	printf("dst: %s\n", dst);
}*/