/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_checker_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmanuel- <cmanuel-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 17:04:43 by cmanuel-          #+#    #+#             */
/*   Updated: 2026/04/15 17:06:55 by cmanuel-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static int	is_valid(t_map *map, int x, int y, int line_len)
{
	if (y < 0 || y >= map->height || x < 0 || x >= line_len)
		return (0);
	if (x >= line_len || map->grid[y][x] == ' ')
		return (0);
	return (1);
}

static void	flood_fill(t_map *map, int x, int y, int *escaped)
{
	int	line_len;

	line_len = ft_strlen(map->grid[y]);
	if (*escaped)
		return ;
	if (!is_valid(map, x, y, line_len))
	{
		*escaped = 1;
		return ;
	}
	if (map->grid[y][x] == '1' || map->grid[y][x] == 'V')
		return ;
	map->grid[y][x] = 'V';
	flood_fill(map, x + 1, y, escaped);
	flood_fill(map, x - 1, y, escaped);
	flood_fill(map, x, y + 1, escaped);
	flood_fill(map, x, y - 1, escaped);
}

static int	check_flood_fill_result(t_map *map)
{
	int	i;
	int	j;

	i = 0;
	while (map->grid[i])
	{
		j = 0;
		while (map->grid[i][j])
		{
			if (map->grid[i][j] == '0' || map->grid[i][j] == 'A'
				|| map->grid[i][j] == 'N' ||
				map->grid[i][j] == 'S' || map->grid[i][j] == 'E'
					|| map->grid[i][j] == 'W')
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}

void	print_map(t_map *map)
{
	int	i;

	i = 0;
	while (map->grid[i])
	{
		printf("%s\n", map->grid[i]);
		i++;
	}
}

int	check_borders(t_map *map)
{
	t_map	*copy;
	int		escaped;
	int		result;

	copy = copy_map(map);
	if (!copy)
		return (0);
	escaped = 0;
	flood_fill(copy, map->player_initial_x, map->player_initial_y, &escaped);
	if (escaped)
		result = 0;
	else
		result = !check_flood_fill_result(copy);
	clean_map(copy);
	return (result);
}
