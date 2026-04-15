/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 12:42:18 by seilkiv           #+#    #+#             */
/*   Updated: 2026/04/13 17:25:45 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static int	count_map_lines(char *filename)
{
	int		fd;
	char	*line;
	int		count;
	int		map_started;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (perror("Error"), -1);
	count = 0;
	map_started = 0;
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		strip_line_end(line);
		if (!map_started && is_map_line(line))
			map_started = 1;
		if (map_started)
			count++;
		free(line);
	}
	return (close(fd), count);
}

static t_map	*init_map_data(char *filename)
{
	t_map	*map;
	int		map_lines;

	map = malloc(sizeof(t_map));
	if (!map)
		return (NULL);
	ft_memset(map, 0, sizeof(t_map));
	map->floor_color = -1;
	map->ceiling_color = -1;
	map_lines = count_map_lines(filename);
	if (map_lines <= 2)
		return (free(map), NULL);
	map->height = map_lines;
	map->grid = malloc(sizeof(char *) * (map->height + 1));
	if (!map->grid)
		return (free(map), NULL);
	return (ft_memset(map->grid, 0, sizeof(char *) * (map->height + 1)), map);
}

t_map	*parse_map(char *filename)
{
	t_map	*map;
	t_map	*filled_map;

	map = init_map_data(filename);
	if (!map)
		return (NULL);
	filled_map = fill_map(map, filename);
	if (!filled_map)
		return (clean_map(map), ft_putstr_fd("Error reading map/textures\n", 2),
			NULL);
	map = filled_map;
	if (check_if_map_is_valid(map, filename) == 0)
		return (clean_map(map), NULL);
	return (map);
}
