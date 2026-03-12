/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_checker_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 13:49:31 by seilkiv           #+#    #+#             */
/*   Updated: 2026/03/12 16:44:50 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static int     check_name(char *filename)
{
    size_t  len;
    char    *cub;

    len = ft_strlen(filename);
    cub = ft_strrchr(filename, '.');
    if (len < 5 || !cub || ft_strcmp(cub, ".cub") != 0)
        return (0);
    return (1);
}

static int	check_format(t_map *map)
{
    if (map->height <= 2 || map->width <= 2)
        return (0);
    return (1);
}

static int  check_characters(t_map *map)
{
    int i;
    int j;
    int player_count = 0;
    int exit_count = 0;

    map->player_initial_dir = 'E';
    i = 0;
    while (map->grid[i])
    {
        j = 0;
        while (map->grid[i][j])
        {
            if (map->grid[i][j] == 'N' || map->grid[i][j] == 'S' || map->grid[i][j] == 'E' || map->grid[i][j] == 'W')
            {
                player_count++;
                map->player_initial_x = j;
                map->player_initial_y = i;
                map->player_initial_dir = map->grid[i][j];
                map->grid[i][j] = '0';
            }
            else if (map->grid[i][j] == 'A')
            {
                exit_count++;
                map->exit_x = j;
                map->exit_y = i;
                map->grid[i][j] = '0';
            }
            else if (map->grid[i][j] != '0' && map->grid[i][j] != '1' && map->grid[i][j] != ' ')
                return (0);
            j++;
        }
        i++;
    }
    if (player_count != 1 || exit_count != 1)
        return (0);
    return (1);
}

int     check_if_map_is_valid(t_map *map, char *filename)
{
    if (check_name(filename) == 0)
        return (ft_putstr_fd("Invalid file name!\n", 2), 0);
    if (check_format(map) == 0)
        return (ft_putstr_fd("Invalid map format!\n", 2), 0);
    if (check_characters(map) == 0)
        return (ft_putstr_fd("Invalid characters or player in map!\n", 2), 0);
    printf("%d\n", map->height); //debug
    printf("%d\n", map->width); //debug
    if (check_borders(map) == 0)
        return (ft_putstr_fd("Invalid map borders!\n", 2), 0);
    return (1);
}