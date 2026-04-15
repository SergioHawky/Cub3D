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

int	is_map_line(char *line)
{
	int	i;

	i = 0;
	while (line[i] && is_blank_char(line[i]))
		i++;
	if (!line[i])
		return (0);
	if ((!ft_strncmp(&line[i], "NO", 2) || !ft_strncmp(&line[i], "SO", 2)
			|| !ft_strncmp(&line[i], "WE", 2) || !ft_strncmp(&line[i], "EA", 2))
		&& is_blank_char(line[i + 2]))
		return (0);
	if (line[i] == '0' || line[i] == '1' || line[i] == 'N' || line[i] == 'S'
		|| line[i] == 'E' || line[i] == 'W' || line[i] == 'A')
		return (1);
	return (0);
}

int	is_color_line(char *line)
{
	int	i;

	i = 0;
	while (line[i] && is_blank_char(line[i]))
		i++;
	if ((line[i] == 'F' || line[i] == 'C') && (line[i + 1] == ' ' || line[i
				+ 1] == '\t'))
		return (1);
	return (0);
}

static char	**get_texture_slot(t_map *map, char *p)
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

int	parse_texture_line(t_map *map, char *line)
{
	char	*path;
	char	*p;
	char	**slot;

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
