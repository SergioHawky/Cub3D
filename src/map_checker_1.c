/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_checker_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 13:49:31 by seilkiv           #+#    #+#             */
/*   Updated: 2026/04/15 17:09:59 by cmanuel-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static void	handle_player_char(t_map *map, int i, int j, int *p_count)
{
	(*p_count)++;
	map->player_initial_x = j;
	map->player_initial_y = i;
	map->player_initial_dir = map->grid[i][j];
	map->grid[i][j] = '0';
}

static void	handle_exit_char(t_map *map, int i, int j, int *e_count)
{
	(*e_count)++;
	map->exit_x = j;
	map->exit_y = i;
	map->grid[i][j] = '0';
}

static int	scan_row(t_map *map, int row, int *p_cnt, int *e_cnt)
{
	int	j;

	j = 0;
	while (map->grid[row][j])
	{
		if (map->grid[row][j] == 'N' || map->grid[row][j] == 'S'
			|| map->grid[row][j] == 'E' || map->grid[row][j] == 'W')
			handle_player_char(map, row, j, p_cnt);
		else if (map->grid[row][j] == 'A')
			handle_exit_char(map, row, j, e_cnt);
		else if (map->grid[row][j] != '0' && map->grid[row][j] != '1'
				&& map->grid[row][j] != ' ')
			return (0);
		j++;
	}
	return (1);
}

static int	check_characters(t_map *map)
{
	int	i;
	int	player_count;
	int	exit_count;

	player_count = 0;
	exit_count = 0;
	map->player_initial_dir = 'E';
	i = 0;
	while (map->grid[i])
	{
		if (scan_row(map, i, &player_count, &exit_count) == 0)
			return (0);
		i++;
	}
	if (player_count != 1 || exit_count != 1)
		return (0);
	return (1);
}

int	check_if_map_is_valid(t_map *map, char *filename)
{
	size_t	len;
	char	*cub;

	len = ft_strlen(filename);
	cub = ft_strrchr(filename, '.');
	if (len < 5 || !cub || ft_strcmp(cub, ".cub") != 0)
		return (ft_putstr_fd("Invalid file name!\n", 2), 0);
	if (map->height <= 2 || map->width <= 2)
		return (ft_putstr_fd("Invalid map format/size!\n", 2), 0);
	if (map->height > MAX_VISIBLE_MAP_SIDE || map->width > MAX_VISIBLE_MAP_SIDE)
		return (ft_putstr_fd("Invalid map format/size!\n", 2), 0);
	if (check_characters(map) == 0)
		return (ft_putstr_fd("Invalid characters or player in map!\n", 2), 0);
	if (check_borders(map) == 0)
		return (ft_putstr_fd("Invalid map borders!\n", 2), 0);
	return (1);
}
