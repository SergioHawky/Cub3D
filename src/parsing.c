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

int	calc_width(t_map *map)
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

static int	textures_are_set(t_map *map)
{
    if (!map->textures.no || !map->textures.so
        || !map->textures.we || !map->textures.ea)
        return (0);
    if (map->floor_color == -1 || map->ceiling_color == -1)
        return (0);
    return (1);
}

static int	parse_before_map(t_map *map, char *line, int *map_started)
{
    char    *trimmed;
    int     tex_result;
    int     color_result;
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

static t_map	*init_map_data(char *filename)
{
    t_map   *map;
    int     map_lines;

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

static int	process_map_line(t_map *map, char *line, int *map_started, int *i)
{
    int status;

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
    if (!textures_are_set(map))
        return (close(fd), NULL);
    return (close(fd), map);
}

static t_map	*fill_map(t_map *map, char *filename)
{
    int		fd;
    char	*line;
    int		i;
    int		map_started;
    int     status;
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

t_map   *parse_map(char *filename)
{
    t_map *map;
    t_map *filled_map;

    map = init_map_data(filename);
    if (!map)
        return (NULL);
    filled_map = fill_map(map, filename);
    if (!filled_map)
        return (clean_map(map), ft_putstr_fd("Error reading map/textures\n", 2), NULL);
    map = filled_map;
    if (check_if_map_is_valid(map, filename) == 0)
	    return (clean_map(map), NULL);
    return (map);
}
