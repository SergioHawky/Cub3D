/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 04:44:25 by seilkiv           #+#    #+#             */
/*   Updated: 2026/03/20 04:45:56 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	key_press_handler(int keysym, t_data *data)
{
    if (keysym == XK_Escape)
	    close_handler(data);
    else if (keysym == XK_w)
	    data->keys[KEY_W] = 1;
    else if (keysym == XK_s)
	    data->keys[KEY_S] = 1;
    else if (keysym == XK_a)
	    data->keys[KEY_A] = 1;
    else if (keysym == XK_d)
	    data->keys[KEY_D] = 1;
    else if (keysym == XK_Left || keysym == XK_h)
        rotate_player(data, -data->player.rot_speed);
    else if (keysym == XK_Right || keysym == XK_l)
        rotate_player(data, data->player.rot_speed);
    return (0);
}

int	key_release_handler(int keysym, t_data *data)
{
    if (keysym == XK_w)
	data->keys[KEY_W] = 0;
    else if (keysym == XK_s)
	data->keys[KEY_S] = 0;
    else if (keysym == XK_a)
	data->keys[KEY_A] = 0;
    else if (keysym == XK_d)
	data->keys[KEY_D] = 0;
    return (0);
}

int	close_handler(t_data *data)
{
    // does cub3d need "mlx_destroy_image()" function?.
    // if so... what could be the "fractal->img.img_ptr"?
    mlx_destroy_image(data->mlx, data->img.img_ptr);

    mlx_destroy_window(data->mlx, data->win);
    if (data->map)
        clean_map(data->map);
    mlx_destroy_display(data->mlx);
    free(data->mlx);
    exit(EXIT_SUCCESS);
}