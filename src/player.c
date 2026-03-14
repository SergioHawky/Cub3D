/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 15:35:28 by seilkiv           #+#    #+#             */
/*   Updated: 2026/03/13 15:40:43 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void init_player(t_data *data)
{
    data->player.x = data->map->player_initial_x + 0.5f;
    data->player.y = data->map->player_initial_y + 0.5f;
    data->player.move_speed = 0.20f;
    data->player.rot_speed = 0.20f;
    if (data->map->player_initial_dir == 'N')
    {
        data->player.dir_x = 0.0f;
        data->player.dir_y = -1.0f;
    }
    else if (data->map->player_initial_dir == 'S')
    {
        data->player.dir_x = 0.0f;
        data->player.dir_y = 1.0f;
    }
    else if (data->map->player_initial_dir == 'W')
    {
        data->player.dir_x = -1.0f;
        data->player.dir_y = 0.0f;
    }
    else
    {
        data->player.dir_x = 1.0f;
        data->player.dir_y = 0.0f;
    }
}

void    move_player(t_data *data, float forward, float strafe)
{
    float move_x;
    float move_y;

    move_x = 0.0f;
    move_y = 0.0f;
    move_x += data->player.dir_x * data->player.move_speed * forward;
    move_y += data->player.dir_y * data->player.move_speed * forward;
    move_x += -data->player.dir_y * data->player.move_speed * strafe;
    move_y += data->player.dir_x * data->player.move_speed * strafe;
    if (!collision(data, data->player.x + move_x, data->player.y))
        data->player.x += move_x;
    if (!collision(data, data->player.x, data->player.y + move_y))
        data->player.y += move_y;
}

void rotate_player(t_data *data, float angle)
{
    float old_dir_x;
    float cos_a;
    float sin_a;

    old_dir_x = data->player.dir_x;
    cos_a = cosf(angle);
    sin_a = sinf(angle);
    data->player.dir_x = data->player.dir_x * cos_a - data->player.dir_y * sin_a;
    data->player.dir_y = old_dir_x * sin_a + data->player.dir_y * cos_a;
}

static void draw_player_direction(t_data *data, int cx, int cy, int r)
{
    float dir_x;
    float dir_y;
    int len;
    int i;

    dir_x = data->player.dir_x;
    dir_y = data->player.dir_y;
    len = r + 8;
    i = 0;
    while (i <= len)
    {
        ft_pixel_put(cx + (dir_x * i), cy + (dir_y * i), &data->img, WHITE);
        i++;
    }
}

void    draw_player(t_data *data)
{
    int cx;
    int cy;
    int r;
    int dx;
    int dy;

    r = PX / 2 - 10;
    cx = (int)(data->player.x * PX);
    cy = (int)(data->player.y * PX);
    dy = -r;
    while (dy <= r)
    {
        dx = -r;
        while (dx <= r)
        {
            if (dx * dx + dy * dy <= r * r)
                ft_pixel_put(cx + dx, cy + dy, &data->img, PINK);
            dx++;
        }
        dy++;
    }
    draw_player_direction(data, cx, cy, r);
}