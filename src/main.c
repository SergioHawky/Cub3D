/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 11:35:59 by seilkiv           #+#    #+#             */
/*   Updated: 2026/03/06 14:54:56 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	init_player(t_player *player)
{
    player->pos_x = 0;
    player->pos_y = 0;
}

// void	init_map(t_map *map)
// {
//     // map->grid = NULL;
//     map->width = 0;
//     printf("here\n");
//     map->height = 0;
//     // map->player = 0;
//     map->player_x = 0;
//     map->player_y = 0;
// }

int 	init_data(char **argv, t_data *data)
{
    data->mlx = 0;
    data->win = 0;
    data->win_height = HEIGHT;
    data->win_width = WIDTH;


    init_player(&data->player);
    // init_map(data->map);

    // init_img(&data->img); // probably not needed

    data->map = parse_map(argv[1]);
    if (!data->map)
        return(print_message_and_free(ERROR_PARSING, data, 2), 0);
    return (1);
}

int init_mlx(t_data *data)
{
    data->mlx = mlx_init();
    if (!data->mlx)
        return(print_message_and_free(ERROR_MLX_INIT, data, 2), 0);

    // WIDTH e HEIGHT são macros definidos no cub3D.h header
    data->win = mlx_new_window(data->mlx, WIDTH, HEIGHT, "cub3D");
    if (!data->win)
        return(print_message_and_free(ERROR_MLX_WIN, data, 2), 0);
    
    data->img.img_ptr = mlx_new_image(data->mlx, WIDTH, HEIGHT);
    if (!data->img.img_ptr)
        return(print_message_and_free("ERROR on mlx_new_image()", data, 2), 0);

    data->img.addr= mlx_get_data_addr(data->img.img_ptr, &data->img.bpp, &data->img.line_len, &data->img.endian);
    return (1);
}

int main(int argc, char **argv)
{
    t_data data;

    if (argc != 2)
        return (print_message_and_free(ERROR_INVALID, NULL, 2), 1);

    if (!init_data(argv, &data) || !init_mlx(&data))
	return(1);
    
    events_init(&data);
    game_render(&data);
    mlx_loop(data.mlx);

    return (0);
}
