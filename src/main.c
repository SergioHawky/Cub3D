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

int close_window(t_data *data)
{
    mlx_destroy_window(data->mlx, data->win);
    if (data->map)
        clean_map(data->map);
    free(data);
    exit(0);
}

int main(int argc, char **argv)
{
    t_data *data;
    
    (void)argv;
    if (argc != 2)
        return (print_message_and_free("Invalid number of arguments!\n", NULL, 2), 1);
    data = malloc(sizeof(t_data));
    if (!data)
        return (print_message_and_free("Memory allocation failed\n", NULL, 2), 1);

    data->map = parse_map(argv[1]);
    if (!data->map)
        return(print_message_and_free("Map parsing failed\n", data, 2), 1);
    data->mlx = mlx_init();
    if (!data->mlx)
        return(print_message_and_free("MLX init failed\n", data, 2), 1);

    data->win = mlx_new_window(data->mlx, 1024, 512, "cub3D");                 // definit tamanho da janela 1900x600
    if (!data->win)
        return(print_message_and_free("Window creation failed\n", data, 2), 1);
    mlx_hook(data->win, 17, 0, close_window, data);
    mlx_loop(data->mlx);
    return 0;
}