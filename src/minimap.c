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

static int  is_wall(t_data *data, float x, float y)
{
    int ix;
    int iy;

    ix = (int)x;
    iy = (int)y;
    if (ix < 0 || iy < 0 || iy >= data->map->height || ix >= data->map->width)
        return (1);
    return (data->map->grid[iy][ix] == '1');
}

int    collision(t_data *data, float new_x, float new_y)
{
    float m;

    m = 0.2f;
    if (is_wall(data, new_x - m, new_y - m))
        return (1);
    if (is_wall(data, new_x + m, new_y - m))
        return (1);
    if (is_wall(data, new_x - m, new_y + m))
        return (1);
    if (is_wall(data, new_x + m, new_y + m))
        return (1);
    return (0);
}