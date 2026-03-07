/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 10:27:41 by seilkiv           #+#    #+#             */
/*   Updated: 2024/11/07 13:41:43 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// This function duplicates a string and returns the duplicate

char	*ft_strdup(const char *s)
{
	char	*p;
	size_t	i;

	p = (char *)malloc((ft_strlen(s) + 1) * sizeof(char));
	if (!p)
	{
		return (NULL);
	}
	i = 0;
	while (s[i])
	{
		p[i] = s[i];
		i++;
	}
	p[i] = '\0';
	return (p);
}
