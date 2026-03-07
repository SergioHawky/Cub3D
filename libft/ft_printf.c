/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 13:17:49 by seilkiv           #+#    #+#             */
/*   Updated: 2025/10/20 13:17:49 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	print_format(char specifier, va_list ap)
{
	if (specifier == 'c')
		return (ft_print_char(va_arg(ap, int)));
	else if (specifier == 's')
		return (ft_print_str(va_arg(ap, char *)));
	else if (specifier == 'p')
		return (ft_print_pt(va_arg(ap, void *)));
	else if (specifier == 'd' || specifier == 'i')
		return (ft_print_nbr(va_arg(ap, int)));
	else if (specifier == 'u')
		return (ft_print_unsigned(va_arg(ap, unsigned int)));
	else if (specifier == 'x')
		return (ft_print_hex(va_arg(ap, unsigned int), 0));
	else if (specifier == 'X')
		return (ft_print_hex(va_arg(ap, unsigned int), 1));
	else if (specifier == '%')
		return (write(1, "%", 1));
	return (0);
}

int	ft_printf(const char *format, ...)
{
	va_list	ap;
	int		counter;

	counter = 0;
	va_start(ap, format);
	while (*format)
	{
		if (*format == '%')
			counter += print_format(*(++format), ap);
		else
			counter += write(1, format, 1);
		format++;
	}
	va_end(ap);
	return (counter);
}

/*int main()
{
	int count;
	int mycount;

	count = printf("Ola meu nome é %s, com %c tenho %d anos, agora
			%%em hexadecimal %x \n", "Sergio", 'S', 20, 20);
	printf("A string tem %d characters\n", count);

	mycount = ft_printf("Ola meu nome é %s, com %c tenho %d anos, agora
			%%em hexadecimal %x \n", "Sergio", 'S', 20, 20);
	ft_printf("A string tem %d characters\n", mycount);

	return (0);
}*/