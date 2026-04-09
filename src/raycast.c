/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 17:23:32 by seilkiv           #+#    #+#             */
/*   Updated: 2026/04/09 17:14:35 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static void init_ray(t_data *data, t_ray *ray, int x)
{
    t_player *p = &data->player;

    float camera_x = 2.0f * x / WIDTH - 1.0f;

    // direção deste raio = dir + plane * camera_x
    ray->ray_dir_x = p->dir_x + p->plane_x * camera_x;
    ray->ray_dir_y = p->dir_y + p->plane_y * camera_x;

    // tile onde o player está
    ray->map_x = (int)p->x;
    ray->map_y = (int)p->y;

    // distância entre cruzamentos consecutivos de grid
    // (se ray_dir for 0, a distância é infinita — nunca cruza esse eixo)
    if (ray->ray_dir_x == 0)
        ray->delta_dist_x = 1e30f;
    else
        ray->delta_dist_x = fabsf(1.0f / ray->ray_dir_x);
    if (ray->ray_dir_y == 0)
        ray->delta_dist_y = 1e30f;
    else
        ray->delta_dist_y = fabsf(1.0f / ray->ray_dir_y);

    // direção de avanço e distância até ao primeiro cruzamento
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

static void perform_dda(t_data *data, t_ray *ray)
{
    int hit = 0;

    while (!hit)
    {
        // avança para o cruzamento mais próximo (X ou Y)
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
        if (is_wall(data, (float)ray->map_x, (float)ray->map_y))
            hit = 1;
    }
    // distância perpendicular — evita fisheye
    if (ray->side == 0)
        ray->perp_dist = ray->side_dist_x - ray->delta_dist_x;
    else
        ray->perp_dist = ray->side_dist_y - ray->delta_dist_y;
}

static void draw_column(t_data *data, t_ray *ray, int x)
{
    int line_height;
    int draw_start;
    int draw_end;
    t_img	*texture;
    float	wall_x;
    float	step;
    float	tex_pos;
    int		tex_x;
    int		tex_y;
    int y;

    if (ray->perp_dist < 0.001f)
        ray->perp_dist = 0.001f;
    line_height = (int)(HEIGHT / ray->perp_dist);
    if (line_height < 1)
        line_height = 1;
    if (line_height > HEIGHT * 4)
        line_height = HEIGHT * 4;
    draw_start = HEIGHT / 2 - line_height / 2;
    draw_end = HEIGHT / 2 + line_height / 2;

    // clamp para não sair do ecrã
    if (draw_start < 0)
        draw_start = 0;
    if (draw_end >= HEIGHT)
        draw_end = HEIGHT - 1;

    texture = get_wall_texture(data, ray);
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

    step = (float)texture->height / (float)line_height;
    tex_pos = (draw_start - HEIGHT / 2 + line_height / 2) * step;

    y = 0;
    while (y < draw_start)  // teto
    {
        ft_pixel_put(x, y, &data->img, data->map->ceiling_color);
        y++;
    }
    while (y <= draw_end)   // parede
    {
        tex_y = (int)tex_pos;
        if (tex_y < 0)
            tex_y = 0;
        if (tex_y >= texture->height)
            tex_y = texture->height - 1;
        ft_pixel_put(x, y, &data->img, get_texture_pixel(texture, tex_x, tex_y));
        tex_pos += step;
        y++;
    }
    while (y < HEIGHT)      // chão
    {
        ft_pixel_put(x, y, &data->img, data->map->floor_color);
        y++;
    }
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