/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_checker_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 14:42:25 by seilkiv           #+#    #+#             */
/*   Updated: 2026/03/07 16:31:00 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static int  is_valid(t_map *map, int x, int y)
{
    int line_len;

    if (y < 0 || y >= map->height)
        return (0);
    if (x < 0)
        return (0);
    line_len = ft_strlen(map->grid[y]);
    if (x >= line_len)
        return (0);
    if (map->grid[y][x] == ' ')
        return (0);
    return (1);
}
static void    flood_fill(t_map *map, int x, int y)
{
    if (!is_valid(map, x, y))
        return;
    if (map->grid[y][x] == '1' || map->grid[y][x] == 'V')
        return;
    map->grid[y][x] = 'V';
    flood_fill(map, x + 1, y);
    flood_fill(map, x - 1, y);
    flood_fill(map, x, y + 1);
    flood_fill(map, x, y - 1);
}

static int  check_flood_fill_result(t_map *map)
{
    int i;
    int j;

    i = 0;
    while(map->grid[i])
    {
        j = 0;
        while(map->grid[i][j])
        {
            if(map->grid[i][j] == '0' || map->grid[i][j] == 'A' || map->grid[i][j] == 'N' ||
               map->grid[i][j] == 'S' || map->grid[i][j] == 'E' || map->grid[i][j] == 'W')
                return (1);
            j++;
        }
        i++;
    }
    return (0);
}

int  check_borders(t_map *map)
{
    t_map *copy;
    int   result;

    copy = copy_map(map);
    if (!copy)
        return (0);
    flood_fill(copy, map->player_x, map->player_y);
    result = !check_flood_fill_result(copy);
    clean_map(copy);
    return(result);
}