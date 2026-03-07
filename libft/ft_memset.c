/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 15:19:25 by seilkiv           #+#    #+#             */
/*   Updated: 2024/11/14 15:11:26 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// This function will set n number of bytes of the 
// string str to the ascci char c
// It returns a pointer to the start of str

void	*ft_memset(void *s, int c, size_t n)
{
	char	*p;

	p = s;
	while (n > 0)
	{
		*p = c;
		n--;
		p++;
	}
	return (s);
}
/*int main()
{
	char buffer[10] = "123456789";

	// Preenche o buffer com o caractere 'A'
	ft_memset(buffer, 'A', 3);

	// Adiciona o terminador nulo no final da string
	buffer[9] = '\0';

	printf("Buffer preenchido: %s\n", buffer);
	return (0);
}*/