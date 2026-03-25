/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 main.c												:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: seilkiv <seilkiv@student.42lisboa.com>		+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/03/05 11:35:59 by seilkiv		   #+#	  #+#			  */
/*	 Updated: 2026/03/12 17:14:54 by seilkiv		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "cub3D.h"

int		init_data(char **argv, t_data *data)
{
	data->map = parse_map(argv[1]);
	if (!data->map)
		return(print_message_and_free(ERROR_PARSING, data, 2), 0);
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
	data->img.addr = mlx_get_data_addr(data->img.img_ptr, &data->img.bpp,
			&data->img.line_len, &data->img.endian);
	init_player(data);
	events_init(data);

	return (1);
}

int	main(int argc, char **argv)
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
