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

static void	draw_mm_tile(t_data *data, int tx, int ty, int color)
{
	int	x;
	int	y;
	int	px;
	int	py;

	x = MM_OFF_X + tx * MM_TILE;
	y = MM_OFF_Y + ty * MM_TILE;
	py = 0;
	while (py < MM_TILE)
	{
		px = 0;
		while (px < MM_TILE)
		{
			if (x + px >= 0 && x + px < WIDTH && y + py >= 0 && y + py < HEIGHT)
				ft_pixel_put(x + px, y + py, &data->img, color);
			px++;
		}
		py++;
	}
}

static void	draw_mm_player(t_data *data)
{
	int		cx;
	int		cy;
	float	t;
	int		px;
	int		py;

	cx = MM_OFF_X + (int)(data->player.x * MM_TILE);
	cy = MM_OFF_Y + (int)(data->player.y * MM_TILE);
	ft_pixel_put(cx, cy, &data->img, PINK);
	ft_pixel_put(cx - 1, cy, &data->img, PINK);
	ft_pixel_put(cx + 1, cy, &data->img, PINK);
	ft_pixel_put(cx, cy - 1, &data->img, PINK);
	ft_pixel_put(cx, cy + 1, &data->img, PINK);
	t = 0.0f;
	while (t < MM_TILE * 2.0f)
	{
		px = cx + (int)(data->player.dir_x * t);
		py = cy + (int)(data->player.dir_y * t);
		if (px >= 0 && px < WIDTH && py >= 0 && py < HEIGHT)
			ft_pixel_put(px, py, &data->img, YELLOW);
		t += 0.5f;
	}
}

void	draw_minimap(t_data *data)
{
	int	i;
	int	j;
	int	color;

	i = 0;
	while (data->map->grid[i])
	{
		j = 0;
		while (data->map->grid[i][j])
		{
			if (data->map->grid[i][j] == '1')
				color = WHITE;
			else if (data->map->grid[i][j] == '0')
				color = UBUNTU;
			else
				color = BLACK;
			draw_mm_tile(data, j, i, color);
			j++;
		}
		i++;
	}
	cast_rays(data);
	draw_mm_player(data);
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