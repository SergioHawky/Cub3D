/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 15:35:28 by seilkiv           #+#    #+#             */
/*   Updated: 2026/03/12 16:44:50 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static void draw_player_direction(t_data *data, int cx, int cy, int r)
{
    int dir_x;
    int dir_y;
    int len;
    int i;

    if (data->map->player_initial_dir == 'N')
    {
        dir_x = 0;
        dir_y = -1;
    }
    else if (data->map->player_initial_dir == 'S')
    {
        dir_x = 0;
        dir_y = 1;
    }
    else if (data->map->player_initial_dir == 'W')
    {
        dir_x = -1;
        dir_y = 0;
    }
    else if (data->map->player_initial_dir == 'E')
    {
        dir_x = 1;
        dir_y = 0;
    }
    len = r + 8;
    i = 0;
    while (i <= len)
    {
        ft_pixel_put(cx + dir_x * i, cy + dir_y * i, &data->img, WHITE);
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
    cx = (int)(data->map->player_initial_x * PX) + PX / 2;
    cy = (int)(data->map->player_initial_y * PX) + PX / 2;
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