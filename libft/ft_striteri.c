/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 12:25:51 by seilkiv           #+#    #+#             */
/*   Updated: 2024/11/05 16:01:34 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char *))
{
	size_t	i;

	if (!s || !f)
	{
		return ;
	}
	i = 0;
	while (s[i])
	{
		f(i, &s[i]);
		i++;
	}
}

/*void	to_upper(unsigned int i, char *c)
{
	if (*c >= 'a' && *c <= 'z')
		*c = toupper(*c) + i;
}

int	main(void)
{
	char str[] = "hello world";

	ft_striteri(str, to_upper);
	printf("Resultado: %s\n", str);

	return (0);
}*/