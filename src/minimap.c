/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 15:35:24 by seilkiv           #+#    #+#             */
/*   Updated: 2026/04/06 18:37:41 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int  is_wall(t_data *data, float x, float y)
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

static void 	draw_tile(t_data *data, int x, int y, int color)
{
    int px;
    int py;

    py = 0;
    while (py < PX)
    {
	    px = 0;
	    while (px < PX)
	    {
	        ft_pixel_put(x * PX + px, y * PX + py, &data->img, color);
	        px++;
	    }
	    py++;
    }
}

void draw_minimap(t_data *data)
{
    int i;
    int j;
    int color;

    i = 0;
	while(data->map->grid[i])
	{
		j = 0;
		while(data->map->grid[i][j])
		{
			if (data->map->grid[i][j] == '1')
				color = WHITE;
			else if (data->map->grid[i][j] == '0')
				color = UBUNTU;
			else
				color = BLACK;
			draw_tile(data, j, i, color);
			j++;
		}
		i++;
	}
    render_minimap_rays(data);                      //aqui
    draw_player(data);
}

void    draw_ray_minimap(t_data *data, t_ray *ray, int x)
{
    float	ray_x;
    float	ray_y;
    float	step;
    float	dist;

    if (x % 8 != 0)
        return ;
    ray_x = data->player.x;
    ray_y = data->player.y;
    step = 0.03f;
    dist = 0.0f;
    while (dist < ray->perp_dist)
    {
        ft_pixel_put((int)(ray_x * PX), (int)(ray_y * PX), &data->img, RED);
        ray_x += ray->ray_dir_x * step;
        ray_y += ray->ray_dir_y * step;
        dist += step;
    }
}