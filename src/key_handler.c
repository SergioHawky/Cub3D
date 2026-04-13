/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 04:44:25 by seilkiv           #+#    #+#             */
/*   Updated: 2026/04/06 18:42:41 by seilkiv          ###   ########.fr       */
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
        data->keys[KEY_LEFT] = 1;
    else if (keysym == XK_Right || keysym == XK_l)
        data->keys[KEY_RIGHT] = 1;
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
    else if (keysym == XK_Left || keysym == XK_h)
        data->keys[KEY_LEFT] = 0;
    else if (keysym == XK_Right || keysym == XK_l)
        data->keys[KEY_RIGHT] = 0;
    return (0);
}

int	close_handler(t_data *data)
{
    cleanup_data(data);
    exit(EXIT_SUCCESS);
}