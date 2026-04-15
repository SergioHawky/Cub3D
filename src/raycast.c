/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmanuel- <cmanuel-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 17:23:32 by seilkiv           #+#    #+#             */
/*   Updated: 2026/04/15 23:40:25 by cmanuel-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static void	init_ray_delta(t_ray *ray)
{
	if (ray->ray_dir_x == 0)
		ray->delta_dist_x = 1e30f;
	else
		ray->delta_dist_x = fabsf(1.0f / ray->ray_dir_x);
	if (ray->ray_dir_y == 0)
		ray->delta_dist_y = 1e30f;
	else
		ray->delta_dist_y = fabsf(1.0f / ray->ray_dir_y);
}

static void	init_ray_steps(t_player *p, t_ray *ray)
{
	if (ray->ray_dir_x < 0)
	{
		ray->step_x = -1;
		ray->side_dist_x = (p->x - ray->map_x) * ray->delta_dist_x;
	}
	else
	{
		ray->step_x = 1;
		ray->side_dist_x = (ray->map_x + 1.0f - p->x) * ray->delta_dist_x;
	}
	if (ray->ray_dir_y < 0)
	{
		ray->step_y = -1;
		ray->side_dist_y = (p->y - ray->map_y) * ray->delta_dist_y;
	}
	else
	{
		ray->step_y = 1;
		ray->side_dist_y = (ray->map_y + 1.0f - p->y) * ray->delta_dist_y;
	}
}

static void	init_ray(t_data *data, t_ray *ray, int x)
{
	t_player	*p;
	float		camera_x;

	p = &data->player;
	camera_x = 2.0f * x / WIDTH - 1.0f;
	ray->ray_dir_x = p->dir_x + p->plane_x * camera_x;
	ray->ray_dir_y = p->dir_y + p->plane_y * camera_x;
	ray->map_x = (int)p->x;
	ray->map_y = (int)p->y;
	init_ray_delta(ray);
	init_ray_steps(p, ray);
}

void	render_3d(t_data *data)
{
	t_ray	ray;
	int		x;

	x = 0;
	while (x < WIDTH)
	{
		init_ray(data, &ray, x);
		perform_dda(data, &ray);
		draw_column(data, &ray, x);
		x++;
	}
}

void	render_minimap_rays(t_data *data)
{
	t_ray	ray;
	int		x;

	x = 0;
	while (x < WIDTH)
	{
		init_ray(data, &ray, x);
		perform_dda(data, &ray);
		draw_ray_minimap(data, &ray, x);
		x++;
	}
}
