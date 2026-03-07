/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 12:26:56 by seilkiv           #+#    #+#             */
/*   Updated: 2024/11/07 13:43:46 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// This function searches for the char c in the string str
// It returns a pointer to the last char in finds

static int	matches(char c, char const *set)
{
	while (*set)
		if (*set++ == c)
			return (1);
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	s1_len;
	char	*str;

	if (!s1 || !set)
		return (NULL);
	while (matches(*s1, set))
		s1++;
	s1_len = ft_strlen(s1);
	if (s1_len == 0)
		return (ft_strdup(s1));
	while (matches(s1[s1_len - 1], set))
		s1_len--;
	str = malloc(s1_len + 1);
	if (!str)
		return (NULL);
	ft_strlcpy(str, s1, s1_len + 1);
	return (str);
}
/*#include <stdio.h>
int	main(void)
{
	char *test;

	test = ft_strtrim("12345abc123456", "1234567890");
	printf("%s\n", test);
	free(test);
}*/