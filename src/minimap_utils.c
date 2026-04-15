/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 23:23:20 by seilkiv           #+#    #+#             */
/*   Updated: 2026/04/15 23:37:55 by cmanuel-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	is_wall(t_data *data, float x, float y)
{
	int	ix;
	int	iy;

	ix = (int)x;
	iy = (int)y;
	if (ix < 0 || iy < 0 || iy >= data->map->height || ix >= data->map->width)
		return (1);
	return (data->map->grid[iy][ix] == '1');
}

int	collision(t_data *data, float new_x, float new_y)
{
	float	m;

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

static void	draw_ray_step(t_data *data, t_minimap *mm, float rx, float ry)
{
	int	px;
	int	py;

	px = mm->off_x + (int)(rx * mm->tile_px);
	py = mm->off_y + (int)(ry * mm->tile_px);
	if (px >= mm->off_x && px < mm->off_x + mm->mini_w
		&& py >= mm->off_y && py < mm->off_y + mm->mini_h)
		ft_pixel_put(px, py, &data->img, RED);
}

void	draw_ray_minimap(t_data *data, t_ray *ray, int x)
{
	t_minimap	mm;
	float		ray_x;
	float		ray_y;
	float		step;
	float		dist;

	if (x % 8 != 0)
		return ;
	minimap_get_layout(data, &mm);
	ray_x = data->player.x;
	ray_y = data->player.y;
	step = 0.03f;
	dist = 0.0f;
	while (dist < ray->perp_dist)
	{
		draw_ray_step(data, &mm, ray_x, ray_y);
		ray_x += ray->ray_dir_x * step;
		ray_y += ray->ray_dir_y * step;
		dist += step;
	}
}
