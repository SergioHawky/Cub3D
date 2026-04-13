/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 11:35:59 by seilkiv           #+#    #+#             */
/*   Updated: 2026/04/13 17:13:25 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int main(int argc, char **argv)
{
	t_data	data;

	ft_memset(&data, 0, sizeof(t_data));
	if (argc != 2)
		return (print_message_and_free(ERROR_INVALID, NULL, 2), 1);
	if (!init_data(argv, &data))
		return (1);
	game_render(&data);
	mlx_loop(data.mlx);
	return (0);
}
