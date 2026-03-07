/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 12:26:45 by seilkiv           #+#    #+#             */
/*   Updated: 2024/11/11 16:14:28 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// This function searches for the char c in the string str
// It returns a pointer to the last char in finds

char	*ft_strrchr(const char *s, int c)
{
	const char	*p;

	p = NULL;
	while (*s)
	{
		if (*s == (unsigned char)c)
		{
			p = s;
		}
		s++;
	}
	if ((unsigned char)c == '\0')
	{
		return ((char *)s);
	}
	return ((char *)p);
}

/*int	main(void)
{
	char *s = "Ol1a mundo123!";

	printf("%s\n", ft_strrchr(s, '1'));
	return (0);
}*/