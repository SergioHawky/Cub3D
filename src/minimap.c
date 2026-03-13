/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 15:35:24 by seilkiv           #+#    #+#             */
/*   Updated: 2026/03/13 16:10:50 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int    collision(t_data *data, float new_x, float new_y)
{
    int x;
    int y;

    x = (int)new_x;
    y = (int)new_y;
    if (data->map->grid[y][x] == '1')
        return (1);
    return (0);
}