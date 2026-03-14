
#include "cub3D.h"

void	ft_pixel_put(int x, int y, t_img *img, int color)
{
	int	offset;

	offset = (y * img->line_len) + (x * (img->bpp / 8));
	*(unsigned int *)(img->addr + offset) = color;
}

static void 	draw_tile(t_data *data, int x, int y, int color)
{
    int px;
    int py;

    py = 0;
    while (py < PX)
    {
	px = 0;
	while (px < PX)
	{
	    ft_pixel_put(x * PX + px, y * PX + py, &data->img, color);
	    px++;
	}
	py++;
    }
}

void 	game_render(t_data *data)
{
    int i;
    int j;
    int color;

    i = 0;
	while(data->map->grid[i])
	{
		j = 0;
		while(data->map->grid[i][j])
		{
			if (data->map->grid[i][j] == '1')
				color = WHITE;
			else if (data->map->grid[i][j] == '0')
				color = UBUNTU;
			else
				color = BLACK;
			draw_tile(data, j, i, color);
			j++;
		}
		i++;
	}
    draw_player(data);
	mlx_put_image_to_window(data->mlx, data->win, data->img.img_ptr, 0, 0);
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
	//    else if (keysym == XK_Left || keysym == XK_h)
	// data->keys[KEY_H] = 1;
	//    else if (keysym == XK_Right || keysym == XK_l)
	// data->keys[KEY_L] = 1;

    else if (keysym == XK_Left || keysym == XK_h)
        rotate_player(data, -data->player.rot_speed);
    else if (keysym == XK_Right || keysym == XK_l)
        rotate_player(data, data->player.rot_speed);
    // else if (keysym == XK_w)
    //     move_player(data, 1.0f, 0.0f);
    // else if (keysym == XK_s)
    //     move_player(data, -1.0f, 0.0f);
    // else if (keysym == XK_a)
    //     move_player(data, 0.0f, -1.0f);
    // else if (keysym == XK_d)
    //     move_player(data, 0.0f, 1.0f);
    //  key_zoom_handler(keysym, fractal);
    // if (keysym == XK_equal || keysym == XK_e || keysym == XK_minus
    //  || keysym == XK_q || keysym == XK_1 || keysym == XK_2 || keysym == XK_3
    //  || keysym == XK_4 || keysym == XK_5)
    //  key_iter_handler(keysym, fractal);
    // if (keysym == XK_z || keysym == XK_x || keysym == XK_c || keysym == XK_v)
    //  key_color_handler(keysym, fractal);
    // if (keysym == XK_g || keysym == XK_h || keysym == XK_j || keysym == XK_k
    //  || keysym == XK_l)
    //  key_mods_handler(keysym, fractal);

    // after player and textures are make, add game_render():
    // game_render(data);
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
	//    else if (keysym == XK_Left || keysym == XK_h)
	// data->keys[KEY_H] = 0;
	//    else if (keysym == XK_Right || keysym == XK_l)
	// data->keys[KEY_L] = 0;
    return (0);
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
	//    if (data->keys[KEY_H])
	// rotate_player(data, -data->player.rot_speed);
	//    if (data->keys[KEY_L])
	// rotate_player(data, data->player.rot_speed);
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
