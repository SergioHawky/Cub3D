/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 15:35:28 by seilkiv           #+#    #+#             */
/*   Updated: 2026/03/18 15:28:33 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void init_player(t_data *data)
{
    data->player.x = data->map->player_initial_x + 0.5f;
    data->player.y = data->map->player_initial_y + 0.5f;
    data->player.move_speed = SPEED;
    data->player.rot_speed = ROT_SPEED;
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

void	draw_line(t_player *player, t_data *data, float ray_angle)
{
    float	cos_angle;
    float	sin_angle;
    float	ray_x;
    float	ray_y;
    float	step;

    cos_angle = cosf(ray_angle);
    sin_angle = sinf(ray_angle);
    ray_x = player->x;
    ray_y = player->y;
    step = 0.03f;
    while (!is_wall(data, ray_x, ray_y))
    {
        ft_pixel_put((int)(ray_x * PX), (int)(ray_y * PX), &data->img, RED);
        ray_x += cos_angle * step;
        ray_y += sin_angle * step;
    }
}

void    draw_players_fov(t_data *data)
{
    float		fraction;
    float		start_angle;
    float		player_angle;
    int		i;
    t_player	*player;

    player = &data->player;
    player_angle = atan2(player->dir_y, player->dir_x);
    fraction = FOV / WIDTH;
    start_angle = player_angle - (FOV / 2);
    i = 0;
    while (i < WIDTH)
    {
        draw_line(player, data, start_angle);
        start_angle += fraction;
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
    draw_players_fov(data);
}
