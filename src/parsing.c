/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 12:42:18 by seilkiv           #+#    #+#             */
/*   Updated: 2026/04/09 15:59:57 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static int	is_blank_char(char c)
{
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r'
        || c == '\v' || c == '\f');
}

static void	strip_line_end(char *line)
{
    size_t	len;

    len = ft_strlen(line);
    while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r'))
    {
        line[len - 1] = '\0';
        len--;
    }
}

static char	*trim_spaces(char *str)
{
    size_t	start;
    size_t	end;

    start = 0;
    while (str[start] && is_blank_char(str[start]))
        start++;
    end = ft_strlen(str);
    while (end > start && is_blank_char(str[end - 1]))
        end--;
    return (ft_substr(str, start, end - start));
}

static int	is_map_line(char *line)
{
    int	i;

    i = 0;
    while (line[i] && is_blank_char(line[i]))
        i++;
    if (!line[i])
        return (0);
    if ((!ft_strncmp(&line[i], "NO", 2) || !ft_strncmp(&line[i], "SO", 2)
            || !ft_strncmp(&line[i], "WE", 2)
            || !ft_strncmp(&line[i], "EA", 2))
        && is_blank_char(line[i + 2]))
        return (0);
    if (line[i] == '0' || line[i] == '1' || line[i] == 'N' || line[i] == 'S'
        || line[i] == 'E' || line[i] == 'W' || line[i] == 'A')
        return (1);
    return (0);
}

static int	is_color_line(char *line)
{
    int	i;

    i = 0;
    while (line[i] && is_blank_char(line[i]))
        i++;
    if ((line[i] == 'F' || line[i] == 'C')
        && (line[i + 1] == ' ' || line[i + 1] == '\t'))
        return (1);
    return (0);
}

static int	parse_texture_line(t_map *map, char *line)
{
    char	*path;
    char	*p;
    char	**slot;

    p = line;
    while (*p && is_blank_char(*p))
        p++;
    if (!*p || is_color_line(p))
        return (0);
    slot = NULL;
    if (!ft_strncmp(p, "NO", 2) && is_blank_char(p[2]))
        slot = &map->textures.no;
    else if (!ft_strncmp(p, "SO", 2) && is_blank_char(p[2]))
        slot = &map->textures.so;
    else if (!ft_strncmp(p, "WE", 2) && is_blank_char(p[2]))
        slot = &map->textures.we;
    else if (!ft_strncmp(p, "EA", 2) && is_blank_char(p[2]))
        slot = &map->textures.ea;
    if (!slot)
        return (0);
    p += 2;
    path = trim_spaces(p);
    if (!path || !*path || *slot)
        return (free(path), -1);
    *slot = path;
    return (1);
}

static int	is_only_digits(char *str)
{
    int	i;

    i = 0;
    if (!str || !str[0])
        return (0);
    while (str[i])
    {
        if (str[i] < '0' || str[i] > '9')
            return (0);
        i++;
    }
    return (1);
}

static int	parse_rgb_component(char *token, int *out)
{
    char	*trimmed;
    int		value;

    trimmed = trim_spaces(token);
    if (!trimmed)
        return (-1);
    if (!is_only_digits(trimmed))
        return (free(trimmed), -1);
    value = ft_atoi(trimmed);
    free(trimmed);
    if (value < 0 || value > 255)
        return (-1);
    *out = value;
    return (0);
}

static void	free_split(char **parts)
{
    int	i;

    if (!parts)
        return ;
    i = 0;
    while (parts[i])
    {
        free(parts[i]);
        i++;
    }
    free(parts);
}

static int	parse_color_line(t_map *map, char *line)
{
    char	*p;
    char	*content;
    char	**parts;
    int		red;
    int		green;
    int		blue;

    p = line;
    while (*p && is_blank_char(*p))
        p++;
    if (!((*p == 'F' || *p == 'C') && is_blank_char(*(p + 1))))
        return (0);
    content = trim_spaces(p + 1);
    if (!content)
        return (-1);
    parts = ft_split(content, ',');
    free(content);
    if (!parts || !parts[0] || !parts[1] || !parts[2] || parts[3])
        return (free_split(parts), -1);
    if (parse_rgb_component(parts[0], &red) == -1
        || parse_rgb_component(parts[1], &green) == -1
        || parse_rgb_component(parts[2], &blue) == -1)
        return (free_split(parts), -1);
    if (*p == 'F')
    {
        if (map->floor_color != -1)
            return (free_split(parts), -1);
        map->floor_color = (red << 16) | (green << 8) | blue;
    }
    else
    {
        if (map->ceiling_color != -1)
            return (free_split(parts), -1);
        map->ceiling_color = (red << 16) | (green << 8) | blue;
    }
    return (free_split(parts), 1);
}

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

static t_map	*fill_map(t_map *map, char *filename)
{
    int		fd;
    char	*line;
    char	*trimmed;
    int		i;
    int		map_started;
    int		tex_result;
    int		color_result;

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
        strip_line_end(line);
        if (!map_started)
        {
            if (is_map_line(line))
                map_started = 1;
            else
            {
                tex_result = parse_texture_line(map, line);
                if (tex_result == -1)
                    return (free(line), close(fd), NULL);
                color_result = parse_color_line(map, line);
                if (color_result == -1)
                    return (free(line), close(fd), NULL);
                trimmed = trim_spaces(line);
                if (!trimmed)
                    return (free(line), close(fd), NULL);
                if (!tex_result && !color_result && *trimmed)
                {
                    free(trimmed);
                    return (free(line), close(fd), NULL);
                }
                free(trimmed);
                free(line);
                continue ;
            }
        }
        map->grid[i++] = line;
    }
    map->grid[i] = NULL;
    map->height = i;
    map->width = calc_width(map);
    if (!textures_are_set(map))
        return (close(fd), NULL);
    return (close(fd), map);
}

t_map   *parse_map(char *filename)
{
    t_map *map;
    t_map *filled_map;
    int map_lines;

    map = malloc(sizeof(t_map));
    if (!map)
        return (NULL);
    ft_memset(map, 0, sizeof(t_map));
    map->floor_color = -1;
    map->ceiling_color = -1;
    map_lines = count_map_lines(filename);
    if (map_lines == -1)
        return (free(map), NULL);
    map->height = map_lines;
    if (map->height <=  2)
        return (free(map), NULL);
    map->grid = malloc(sizeof(char *) * (map->height + 1));
    if (!map->grid)
        return (free(map), NULL);
    ft_memset(map->grid, 0, sizeof(char *) * (map->height + 1));
    filled_map = fill_map(map, filename);
    if (!filled_map)
        return (clean_map(map), ft_putstr_fd("Error reading map/textures\n", 2), NULL);
    map = filled_map;
    if (check_if_map_is_valid(map, filename) == 0)
	    return (clean_map(map), NULL);
    return (map);
}
