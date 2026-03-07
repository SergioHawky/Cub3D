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

int 	init_data(char **argv, t_data *data)
{
    data->map = parse_map(argv[1]);
    if (!data->map)
        return(print_message_and_free("Map parsing failed\n", data, 2), 0);
    data->mlx = mlx_init();
    if (!data->mlx)
        return(print_message_and_free("MLX init failed\n", data, 2), 0);

    // WIDTH e HEIGHT são macros definidos no cub3D.h header
    data->win = mlx_new_window(data->mlx, WIDTH, HEIGHT, "cub3D");
    if (!data->win)
        return(print_message_and_free("Window creation failed\n", data, 2), 0);
    return (1);
}

int main(int argc, char **argv)
{
    t_data data;

    if (argc != 2)
        return (print_message_and_free("Invalid number of arguments!\n", NULL, 2), 1);

    if (!init_data(argv, &data))
	return(1);
    events_init(&data);
    mlx_loop(data.mlx);

    return (0);
}
