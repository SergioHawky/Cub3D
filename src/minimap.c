/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 15:35:24 by seilkiv           #+#    #+#             */
/*   Updated: 2026/04/13 17:01:08 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void    minimap_get_layout(t_data *data, int *offset_x, int *offset_y,
    int *tile_px, int *mini_w, int *mini_h)
{
    int tpx_w;
    int tpx_h;

    tpx_w = MINIMAP_SIZE / data->map->width;
    tpx_h = MINIMAP_SIZE / data->map->height;
    *tile_px = tpx_w;
    if (tpx_h < *tile_px)
        *tile_px = tpx_h;
    if (*tile_px < 1)
        *tile_px = 1;
    *mini_w = data->map->width * (*tile_px);
    *mini_h = data->map->height * (*tile_px);
    *offset_x = MINIMAP_MARGIN;
    *offset_y = HEIGHT - *mini_h - MINIMAP_MARGIN;
    if (*offset_y < 0)
        *offset_y = 0;
}

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
    int off_x;
    int off_y;
    int tile_px;
    int mini_w;
    int mini_h;

    minimap_get_layout(data, &off_x, &off_y, &tile_px, &mini_w, &mini_h);
    (void)mini_w;
    (void)mini_h;

    py = 0;
    while (py < tile_px)
    {
	    px = 0;
	    while (px < tile_px)
	    {
            ft_pixel_put(off_x + x * tile_px + px, off_y + y * tile_px + py,
                &data->img, color);
	        px++;
	    }
	    py++;
    }
}

static int  tile_color(t_data *data, int map_x, int map_y)
{
    char c;

    if (map_x < 0 || map_y < 0 || map_y >= data->map->height
        || map_x >= data->map->width)
        return (BLACK);
    c = data->map->grid[map_y][map_x];
    if (c == '1')
        return (WHITE);
    if (c == '0' || c == 'N' || c == 'S' || c == 'E' || c == 'W' || c == 'A')
        return (UBUNTU);
    return (BLACK);
}

void draw_minimap(t_data *data)
{
    int i;
    int j;

    i = 0;
    while (i < data->map->height)
    {
        j = 0;
        while (j < data->map->width)
        {
            draw_tile(data, j, i, tile_color(data, j, i));
            j++;
        }
        i++;
    }
    render_minimap_rays(data);
    draw_player(data);
}

void    draw_ray_minimap(t_data *data, t_ray *ray, int x)
{
    float	ray_x;
    float	ray_y;
    float	step;
    float	dist;
    int     off_x;
    int     off_y;
    int     tile_px;
    int     mini_w;
    int     mini_h;
    int     px;
    int     py;

    if (x % 8 != 0)
        return ;
    minimap_get_layout(data, &off_x, &off_y, &tile_px, &mini_w, &mini_h);
    ray_x = data->player.x;
    ray_y = data->player.y;
    step = 0.03f;
    dist = 0.0f;
    while (dist < ray->perp_dist)
    {
        px = off_x + (int)(ray_x * tile_px);
        py = off_y + (int)(ray_y * tile_px);
        if (px >= off_x && px < off_x + mini_w
            && py >= off_y && py < off_y + mini_h)
            ft_pixel_put(px, py, &data->img, RED);
        ray_x += ray->ray_dir_x * step;
        ray_y += ray->ray_dir_y * step;
        dist += step;
    }
}