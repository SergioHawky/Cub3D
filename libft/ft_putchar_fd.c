/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 12:23:57 by seilkiv           #+#    #+#             */
/*   Updated: 2024/11/05 16:04:05 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

/*int main()
{
	// Imprime "E" na saída de erro (stderr - fd 2)
	ft_putchar_fd('E', 2);
	ft_putchar_fd('r', 2);
	ft_putchar_fd('r', 2);
	ft_putchar_fd('o', 2);
	ft_putchar_fd('r', 2);
	ft_putchar_fd('\n', 2);
	return (0);
}
int	main(void)
{
	// Imprimindo o caractere 'A' no terminal (saída padrão - fd 1)
	ft_putchar_fd('A\n', 1);
	return (0);
}
int	main(void)
{
	int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

	if (fd == -1) {
		write(2, "Erro ao abrir o arquivo.\n", 25);
		return (1);
	}

	// Escreve 'H', 'e', 'l', 'l', 'o' no arquivo
	ft_putchar_fd('H', fd);
	ft_putchar_fd('e', fd);
	ft_putchar_fd('l', fd);
	ft_putchar_fd('l', fd);
	ft_putchar_fd('o', fd);

	// Fecha o arquivo
	close(fd);
	return (0);
}*/