/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 11:27:22 by seilkiv           #+#    #+#             */
/*   Updated: 2024/11/05 11:05:33 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// This function will set n number of bytes of the string str to \0 character

void	ft_bzero(void *s, size_t n)
{
	char	*p;

	p = s;
	while (n > 0)
	{
		*p = '\0';
		p++;
		n--;
	}
}

/*#include <stdio.h>
int	main(void){
	char test[] = "testing string";

	printf("before function: %s\n", test);
	ft_bzero (test, 0);
	printf("after function: %s\n", test);
}*/