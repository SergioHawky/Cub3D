/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_config.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 17:13:03 by seilkiv           #+#    #+#             */
/*   Updated: 2026/04/13 16:05:24 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int is_map_line(char *line)
{
    int i;

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

int is_color_line(char *line)
{
    int i;

    i = 0;
    while (line[i] && is_blank_char(line[i]))
        i++;
    if ((line[i] == 'F' || line[i] == 'C')
        && (line[i + 1] == ' ' || line[i + 1] == '\t'))
        return (1);
    return (0);
}

static char **get_texture_slot(t_map *map, char *p)
{
    if (!ft_strncmp(p, "NO", 2) && is_blank_char(p[2]))
        return (&map->textures.no);
    if (!ft_strncmp(p, "SO", 2) && is_blank_char(p[2]))
        return (&map->textures.so);
    if (!ft_strncmp(p, "WE", 2) && is_blank_char(p[2]))
        return (&map->textures.we);
    if (!ft_strncmp(p, "EA", 2) && is_blank_char(p[2]))
        return (&map->textures.ea);
    return (NULL);
}

int parse_texture_line(t_map *map, char *line)
{
    char    *path;
    char    *p;
    char    **slot;

    p = line;
    while (*p && is_blank_char(*p))
        p++;
    if (!*p || is_color_line(p))
        return (0);
    slot = get_texture_slot(map, p);
    if (!slot)
        return (0);
    p += 2;
    path = trim_spaces(p);
    if (!path || !*path || *slot)
        return (free(path), -1);
    *slot = path;
    return (1);
}

static int  parse_rgb_component(char *token, int *out)
{
    char    *trimmed;
    int     value;

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

static int  parse_rgb_values(char **parts, int *r, int *g, int *b)
{
    if (parse_rgb_component(parts[0], r) == -1
        || parse_rgb_component(parts[1], g) == -1
        || parse_rgb_component(parts[2], b) == -1)
        return (-1);
    return (0);
}

static int  apply_parsed_color(t_map *map, char type, int r, int g, int b)
{
    int color;

    color = (r << 16) | (g << 8) | b;
    if (type == 'F')
    {
        if (map->floor_color != -1)
            return (-1);
        map->floor_color = color;
    }
    else
    {
        if (map->ceiling_color != -1)
            return (-1);
        map->ceiling_color = color;
    }
    return (0);
}

int parse_color_line(t_map *map, char *line)
{
    char    *p;
    char    *content;
    char    **parts;
    int     red;
    int     green;
    int     blue;

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
    if (parse_rgb_values(parts, &red, &green, &blue) == -1)
        return (free_split(parts), -1);
    if (apply_parsed_color(map, *p, red, green, blue) == -1)
        return (free_split(parts), -1);
    return (free_split(parts), 1);
}
