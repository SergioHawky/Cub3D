/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 15:52:14 by seilkiv           #+#    #+#             */
/*   Updated: 2024/11/11 16:01:35 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	**free_all(char **dest, int counter)
{
	while (counter-- > 0)
		free(dest[counter]);
	free(dest);
	return (NULL);
}

static int	count_words(const char *s, char c)
{
	int	counter;
	int	i;

	counter = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] && s[i] == c)
			i++;
		if (s[i] == '\0')
			break ;
		while (s[i] && s[i] != c)
			i++;
		counter++;
	}
	return (counter);
}

static char	**split(char const *s, char c, char **dest, int counter)
{
	int	i;
	int	j;

	j = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] && s[i] == c)
			i++;
		if (s[i] == '\0')
			break ;
		j = i;
		while (s[i] && s[i] != c)
			i++;
		dest[counter] = malloc(sizeof(char) * (i - j + 1));
		if (!dest[counter])
		{
			free_all(dest, counter);
			return (NULL);
		}
		ft_strlcpy(dest[counter], &s[j], (i - j + 1));
		counter++;
	}
	dest[counter] = NULL;
	return (dest);
}

char	**ft_split(char const *s, char c)
{
	char	**dest;
	int		counter;

	counter = 0;
	if (!s)
		return (NULL);
	dest = malloc(sizeof(char *) * (count_words(s, c) + 1));
	if (!dest)
		return (NULL);
	dest = split(s, c, dest, counter);
	return (dest);
}

/*#include <stdio.h>
int	main(void) {
	char str1[] = "Hello world abc 32";
	char delimiter1 = ' ';
	char **result1 = ft_split(str1, delimiter1);

	printf("Separando \"%s\" pelo delimitador '%c'\n", str1, delimiter1);
	for (int i = 0; result1[i] != NULL; i++) {
		printf("Palavra %d: %s\n", i + 1, result1[i]);
		free(result1[i]);  // Liberar cada string alocada
	}
	free(result1);  // Liberar o array principal

	return (0);
}*/