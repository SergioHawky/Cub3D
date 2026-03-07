/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 12:26:36 by seilkiv           #+#    #+#             */
/*   Updated: 2024/11/14 15:12:24 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// This function searches n bytes of the str for the string to find
// It returns a pointer to the found string or null if there wasn't 1

char	*ft_strnstr(const char *str, const char *find, size_t n)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (find[j] == '\0')
	{
		return ((char *)str);
	}
	while (i < n && str[i] != '\0')
	{
		j = 0;
		while (str[i + j] == find[j] && str[i + j] != '\0' && (i + j) < n)
			j++;
		if (find[j] == '\0')
			return ((char *)(str + i));
		i++;
	}
	return (NULL);
}
/*int	main(void)
{
	char s1[40] = "ola mundo . 123 fixe";
	char s2[10] = "undo";
	char *result = ft_strnstr(s1, s2, 8);
	printf("%s\n", result);
}*/