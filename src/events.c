
#include "cub3D.h"

int	close_handler(t_data *data)
{
    // does cub3d need "mlx_destroy_image()" function?.
    // if so... what could be the "fractal->img.img_ptr"?
    // mlx_destroy_image(data->mlx, fractal->img.img_ptr);

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
    // if (keysym == XK_Left || keysym == XK_a || keysym == XK_Right
    //  || keysym == XK_d || keysym == XK_Up || keysym == XK_w
    //  || keysym == XK_Down || keysym == XK_s || keysym == XK_i
    //  || keysym == XK_o || keysym == XK_0)
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

void 	events_init(t_data *data)
{
    mlx_hook(data->win, KeyPress, KeyPressMask, key_press_handler, data);
    mlx_hook(data->win, DestroyNotify, StructureNotifyMask, close_handler, data);
}
