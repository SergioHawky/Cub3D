/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 12:46:06 by seilkiv           #+#    #+#             */
/*   Updated: 2024/11/05 15:59:53 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char	*p;
	size_t	i;

	if (!s || !f)
	{
		return (NULL);
	}
	p = (char *)malloc((ft_strlen(s) + 1) * sizeof(char));
	if (!p)
	{
		return (NULL);
	}
	i = 0;
	while (s[i])
	{
		p[i] = f(i, s[i]);
		i++;
	}
	p[i] = '\0';
	return (p);
}

/*char	to_upper_with_index(unsigned int i, char c)
{
	if (c >= 'a' && c <= 'z') // Converte para maiúscula, se for minúscula
		c = toupper(c);
	return (c + i); // Soma o índice ao caractere
}

int	main(void)
{
	char *original = "abcd";
	char *result;

	// Chama ft_strmapi com a função to_upper_with_index
	result = ft_strmapi(original, to_upper_with_index);

	if (result)
	{
		printf("Original: %s\n", original);
		printf("Modificado: %s\n", result);
		free(result); // Libera a memória alocada
	}
	else
	{
		printf("Erro na alocação de memória!\n");
	}

	return (0);
}*/