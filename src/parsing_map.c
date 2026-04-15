/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_map.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmanuel- <cmanuel-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 23:39:50 by cmanuel-          #+#    #+#             */
/*   Updated: 2026/04/15 23:39:58 by cmanuel-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static int	calc_width(t_map *map)
{
	int	n;
	int	i;
	int	len;

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

static int	parse_before_map(t_map *map, char *line, int *map_started)
{
	char	*trimmed;
	int		tex_result;
	int		color_result;

	if (*map_started)
		return (1);
	if (is_map_line(line))
		return (*map_started = 1, 1);
	tex_result = parse_texture_line(map, line);
	if (tex_result == -1)
		return (-1);
	color_result = parse_color_line(map, line);
	if (color_result == -1)
		return (-1);
	trimmed = trim_spaces(line);
	if (!trimmed)
		return (-1);
	if (!tex_result && !color_result && *trimmed)
		return (free(trimmed), -1);
	return (free(trimmed), 0);
}

static int	process_map_line(t_map *map, char *line, int *map_started, int *i)
{
	int	status;

	strip_line_end(line);
	status = parse_before_map(map, line, map_started);
	if (status == -1)
		return (-1);
	if (status == 0)
		return (free(line), 0);
	map->grid[(*i)++] = line;
	return (1);
}

static t_map	*finish_map_fill(t_map *map, int i, int fd)
{
	map->grid[i] = NULL;
	map->height = i;
	map->width = calc_width(map);
	if (!map->textures.no || !map->textures.so || !map->textures.we
		|| !map->textures.ea)
		return (close(fd), NULL);
	if (map->floor_color == -1 || map->ceiling_color == -1)
		return (close(fd), NULL);
	return (close(fd), map);
}

t_map	*fill_map(t_map *map, char *filename)
{
	int		fd;
	char	*line;
	int		i;
	int		map_started;
	int		status;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (perror("Error:\n"), NULL);
	i = 0;
	map_started = 0;
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		status = process_map_line(map, line, &map_started, &i);
		if (status == -1)
			return (free(line), close(fd), NULL);
		if (status == 0)
			continue ;
	}
	return (finish_map_fill(map, i, fd));
}
