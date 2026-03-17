/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 12:42:18 by seilkiv           #+#    #+#             */
/*   Updated: 2026/03/17 11:07:17 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static int	count_lines(char *filename)
{
	size_t	count;
	int		fd;
	char	*line;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (perror("Error"), -1);
	count = 0;
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		count++;
		free(line);
	}
	return (close(fd), count);
}

int	calc_width(t_map *map)
{
    int n;
    int i;
    int len;

    n = 0;
    i = 0;
    while (map->grid[i])
    {
	    len = ft_strlen(map->grid[i]);
	    if (len > n)
	        n = len;
	    i++;
    }
    return (n);
}

static t_map	*fill_map(t_map *map, char *filename)
{
    int		fd;
    char	*line;
    size_t	i;
    size_t len;

    fd = open(filename, O_RDONLY);
    if (fd < 0)
	    return (perror("Error:\n"), NULL);
    i = 0;
    line = NULL;
    while (1)
    {
	    line = get_next_line(fd);
	    if (!line)
		    break ;
	    len = ft_strlen(line);
	    while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r'))  //mudo ou deixo assim?
	    {
		    line[len - 1] = '\0';
		    len--;
	    }
	    map->grid[i++] = line;
    }
    
    // cg: replaced this with the following calc_width()
    // map->width = ft_strlen(map->grid[0]);
    map->width = calc_width(map);
    map->grid[i] = NULL;
    return (close(fd), map);
}

t_map   *parse_map(char *filename)
{
    t_map *map;

    map = malloc(sizeof(t_map));
    if (!map)
        return (NULL);
    if (count_lines(filename) == -1)
        return (free(map), NULL);
    map->height = count_lines(filename);
    if (map->height <=  2)
        return (free(map), NULL);
    map->grid = malloc(sizeof(char *) * (map->height + 1));
    if (!map->grid)
        return (free(map->grid), free(map), NULL);
    map = fill_map(map, filename);
    if (!map)
	    return (ft_putstr_fd("Error reading map\n", 2), NULL);
    if (check_if_map_is_valid(map, filename) == 0)
	    return (clean_map(map), NULL);
    return (map);
}
