/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 11:40:03 by seilkiv           #+#    #+#             */
/*   Updated: 2026/03/20 04:46:18 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	ft_pixel_put(int x, int y, t_img *img, int color)
{
	int	offset;

	if (x < 0 || y < 0 || x >= WIDTH || y >= HEIGHT)      //aqui
		return ;
	offset = (y * img->line_len) + (x * (img->bpp / 8));
	*(unsigned int *)(img->addr + offset) = color;
}

void 	game_render(t_data *data)
{
    render_3d(data);                                //aqui
    draw_map(data);                                 //aqui
    render_minimap_rays(data);                      //aqui
    draw_player(data);
    mlx_put_image_to_window(data->mlx, data->win, data->img.img_ptr, 0, 0);
}

int game_loop(t_data *data)
{
    if (data->keys[KEY_W])
        move_player(data, SPEED, 0.0f);
    if (data->keys[KEY_S])
	    move_player(data, -SPEED, 0.0f);
    if (data->keys[KEY_A])
	    move_player(data, 0.0f, -SPEED);
    if (data->keys[KEY_D])
	    move_player(data, 0.0f, SPEED);
    game_render(data);
    return (0);
}

void 	events_init(t_data *data)
{
    mlx_hook(data->win, KeyPress, KeyPressMask, key_press_handler, data);
    mlx_hook(data->win, KeyRelease, KeyReleaseMask, key_release_handler, data);
    mlx_hook(data->win, DestroyNotify, StructureNotifyMask, close_handler, data);
    mlx_loop_hook(data->mlx, game_loop, data);
}
