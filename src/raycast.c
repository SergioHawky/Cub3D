/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 17:23:32 by seilkiv           #+#    #+#             */
/*   Updated: 2026/04/13 17:26:31 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static void init_ray_delta(t_ray *ray)
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

static void init_ray_steps(t_player *p, t_ray *ray)
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

static void init_ray(t_data *data, t_ray *ray, int x)
{
    t_player *p = &data->player;
    float camera_x = 2.0f * x / WIDTH - 1.0f;

    ray->ray_dir_x = p->dir_x + p->plane_x * camera_x;
    ray->ray_dir_y = p->dir_y + p->plane_y * camera_x;
    ray->map_x = (int)p->x;
    ray->map_y = (int)p->y;
    init_ray_delta(ray);
    init_ray_steps(p, ray);
}

static void step_dda(t_ray *ray)
{
    if (ray->side_dist_x < ray->side_dist_y)
    {
        ray->side_dist_x += ray->delta_dist_x;
        ray->map_x += ray->step_x;
        ray->side = 0;
    }
    else
    {
        ray->side_dist_y += ray->delta_dist_y;
        ray->map_y += ray->step_y;
        ray->side = 1;
    }
}

static void set_perp_dist(t_ray *ray)
{
    if (ray->side == 0)
        ray->perp_dist = ray->side_dist_x - ray->delta_dist_x;
    else
        ray->perp_dist = ray->side_dist_y - ray->delta_dist_y;
}

static void perform_dda(t_data *data, t_ray *ray)
{
    int hit = 0;

    while (!hit)
    {
        step_dda(ray);
        if (is_wall(data, (float)ray->map_x, (float)ray->map_y))
            hit = 1;
    }
    set_perp_dist(ray);
}

static void set_draw_range(t_ray *ray, int *line_height, int *draw_start, int *draw_end)
{
    if (ray->perp_dist < 0.001f)
        ray->perp_dist = 0.001f;
    *line_height = (int)(HEIGHT / ray->perp_dist);
    if (*line_height < 1)
        *line_height = 1;
    if (*line_height > HEIGHT * 4)
        *line_height = HEIGHT * 4;
    *draw_start = HEIGHT / 2 - *line_height / 2;
    *draw_end = HEIGHT / 2 + *line_height / 2;
    if (*draw_start < 0)
        *draw_start = 0;
    if (*draw_end >= HEIGHT)
        *draw_end = HEIGHT - 1;
}

static int get_tex_x(t_data *data, t_ray *ray, t_img *texture)
{
    float   wall_x;
    int     tex_x;

    if (ray->side == 0)
        wall_x = data->player.y + ray->perp_dist * ray->ray_dir_y;
    else
        wall_x = data->player.x + ray->perp_dist * ray->ray_dir_x;
    wall_x -= floorf(wall_x);
    tex_x = (int)(wall_x * (float)texture->width);
    if ((ray->side == 0 && ray->ray_dir_x > 0)
        || (ray->side == 1 && ray->ray_dir_y < 0))
        tex_x = texture->width - tex_x - 1;
    if (tex_x < 0)
        tex_x = 0;
    if (tex_x >= texture->width)
        tex_x = texture->width - 1;
    return (tex_x);
}

static void draw_column_pixels(t_data *data, int x, int draw_start, int draw_end,
    t_img *texture, int tex_x, float step, int line_height)
{
    int     y;
    int     tex_y;
    float   tex_pos;

    tex_pos = (draw_start - HEIGHT / 2 + line_height / 2) * step;
    y = 0;
    while (y < draw_start)
        ft_pixel_put(x, y++, &data->img, data->map->ceiling_color);
    while (y <= draw_end)
    {
        tex_y = (int)tex_pos;
        if (tex_y < 0)
            tex_y = 0;
        if (tex_y >= texture->height)
            tex_y = texture->height - 1;
        ft_pixel_put(x, y++, &data->img, get_texture_pixel(texture, tex_x, tex_y));
        tex_pos += step;
    }
    while (y < HEIGHT)
        ft_pixel_put(x, y++, &data->img, data->map->floor_color);
}

static void draw_column(t_data *data, t_ray *ray, int x)
{
    int line_height;
    int draw_start;
    int draw_end;
    t_img	*texture;
    float	step;
    int		tex_x;
    set_draw_range(ray, &line_height, &draw_start, &draw_end);

    texture = get_wall_texture(data, ray);
    tex_x = get_tex_x(data, ray, texture);
    step = (float)texture->height / (float)line_height;
    draw_column_pixels(data, x, draw_start, draw_end, texture, tex_x,
        step, line_height);
}

void    render_3d(t_data *data)
{
    t_ray   ray;
    int     x;

    x = 0;
    while (x < WIDTH)
    {
        init_ray(data, &ray, x);
        perform_dda(data, &ray);
        draw_column(data, &ray, x);
        x++;
    }
}

void    render_minimap_rays(t_data *data)
{
    t_ray   ray;
    int     x;

    x = 0;
    while (x < WIDTH)
    {
        init_ray(data, &ray, x);
        perform_dda(data, &ray);
        draw_ray_minimap(data, &ray, x);
        x++;
    }
}
