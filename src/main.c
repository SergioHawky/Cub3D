/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 14:01:40 by seilkiv           #+#    #+#             */
/*   Updated: 2026/03/04 14:01:40 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int main(int argc, char **argv)
{
    t_data data;
    (void)argv;
    if (argc != 2)
        return (print_message("Invalid number of arguments!\n"), 1);

    data.mlx = mlx_init();
    if (!data.mlx)
        return (print_message("MLX init failed\n"), 1);

    data.win = mlx_new_window(data.mlx, 1600, 900, "cub3D");                 // definit tamanho da janela
    if (!data.win)
        return (print_message("Window creation failed\n"), 1);

    mlx_loop(data.mlx);
    return 0;
}