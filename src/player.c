/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 15:35:28 by seilkiv           #+#    #+#             */
/*   Updated: 2026/03/12 16:29:20 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void    draw_player(t_data *data)
{
    int cx;
    int cy;
    int r;
    int dx;
    int dy;

    r = PX / 2 - 8;
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
}