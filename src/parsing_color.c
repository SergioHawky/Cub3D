/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_color.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmanuel- <cmanuel-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 23:39:22 by cmanuel-          #+#    #+#             */
/*   Updated: 2026/04/15 23:39:29 by cmanuel-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

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

static int	parse_rgb_values(char **parts)
{
	int	r;
	int	g;
	int	b;

	if (parse_rgb_component(parts[0], &r) == -1)
		return (-1);
	if (parse_rgb_component(parts[1], &g) == -1)
		return (-1);
	if (parse_rgb_component(parts[2], &b) == -1)
		return (-1);
	return ((r << 16) | (g << 8) | b);
}

static int	apply_parsed_color(t_map *map, char type, int color)
{
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

int	parse_color_line(t_map *map, char *line)
{
	char	*p;
	char	*content;
	char	**parts;
	int		color;

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
	color = parse_rgb_values(parts);
	if (color == -1 || apply_parsed_color(map, *p, color) == -1)
		return (free_split(parts), -1);
	return (free_split(parts), 1);
}
