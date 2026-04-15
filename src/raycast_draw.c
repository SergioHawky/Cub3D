/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_draw.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmanuel- <cmanuel-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 23:24:46 by cmanuel-          #+#    #+#             */
/*   Updated: 2026/04/15 23:40:38 by cmanuel-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static void	set_draw_range(t_ray *ray, t_col *col)
{
	if (ray->perp_dist < 0.001f)
		ray->perp_dist = 0.001f;
	col->line_height = (int)(HEIGHT / ray->perp_dist);
	if (col->line_height < 1)
		col->line_height = 1;
	if (col->line_height > HEIGHT * 4)
		col->line_height = HEIGHT * 4;
	col->draw_start = HEIGHT / 2 - col->line_height / 2;
	col->draw_end = HEIGHT / 2 + col->line_height / 2;
	if (col->draw_start < 0)
		col->draw_start = 0;
	if (col->draw_end >= HEIGHT)
		col->draw_end = HEIGHT - 1;
}

static int	get_tex_x(t_data *data, t_ray *ray, t_img *texture)
{
	float	wall_x;
	int		tex_x;

	if (ray->side == 0)
		wall_x = data->player.y + ray->perp_dist * ray->ray_dir_y;
	else
		wall_x = data->player.x + ray->perp_dist * ray->ray_dir_x;
	wall_x -= floorf(wall_x);
	tex_x = (int)(wall_x * (float)texture->width);
	if ((ray->side == 0 && ray->ray_dir_x > 0) || (ray->side == 1
			&& ray->ray_dir_y < 0))
		tex_x = texture->width - tex_x - 1;
	if (tex_x < 0)
		tex_x = 0;
	if (tex_x >= texture->width)
		tex_x = texture->width - 1;
	return (tex_x);
}

static void	draw_column_pixels(t_data *data, int x, t_col *col)
{
	int		y;
	int		tex_y;
	float	tex_pos;
	int		color;

	tex_pos = (col->draw_start - HEIGHT / 2 + col->line_height / 2) * col->step;
	y = 0;
	while (y < col->draw_start)
		ft_pixel_put(x, y++, &data->img, data->map->ceiling_color);
	while (y <= col->draw_end)
	{
		tex_y = (int)tex_pos;
		if (tex_y < 0)
			tex_y = 0;
		if (tex_y >= col->texture->height)
			tex_y = col->texture->height - 1;
		color = get_texture_pixel(col->texture, col->tex_x, tex_y);
		ft_pixel_put(x, y++, &data->img, color);
		tex_pos += col->step;
	}
	while (y < HEIGHT)
		ft_pixel_put(x, y++, &data->img, data->map->floor_color);
}

void	draw_column(t_data *data, t_ray *ray, int x)
{
	t_col	col;

	set_draw_range(ray, &col);
	col.texture = get_wall_texture(data, ray);
	col.tex_x = get_tex_x(data, ray, col.texture);
	col.step = (float)col.texture->height / (float)col.line_height;
	draw_column_pixels(data, x, &col);
}
