/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 13:18:26 by seilkiv           #+#    #+#             */
/*   Updated: 2025/10/20 13:18:26 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*get_next_line(int fd)
{
	static char	buff[BUFFER_SIZE + 1];
	ssize_t		bytes_read;
	char		*line;

	line = NULL;
	bytes_read = 1;
	while ((fd >= 0 && BUFFER_SIZE > 0) && bytes_read > 0)
	{
		if (buff[0])
		{
			line = ft_strjoin_gnl(line, buff);
			if (!line)
				return (NULL);
			if (line[ft_strlen_gnl(line) - 1] == '\n')
				break ;
		}
		else
		{
			bytes_read = read(fd, buff, BUFFER_SIZE);
			if (bytes_read < 0)
				return (free(line), NULL);
			buff[bytes_read] = '\0';
		}
	}
	return (line);
}
