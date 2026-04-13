/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 11:34:13 by seilkiv           #+#    #+#             */
/*   Updated: 2026/04/13 17:11:20 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	clean_map(t_map *map)
{
	int	i;

    if (!map)
        return;
    free(map->textures.no);
    free(map->textures.so);
    free(map->textures.we);
    free(map->textures.ea);
    if (map->grid)
    {
        i = 0;
        while (map->grid[i])
        {
            free(map->grid[i]);
            i++;
        }
        free(map->grid);
    }
    free(map);
}

void	print_message_and_free(char *message, t_data *data, int fd)
{
	ft_putstr_fd(message, fd);
	if (data)
	{
		if (data->map)
			clean_map(data->map);
	}
}

void    cleanup_data(t_data *data)
{
    int i;

    if (!data)
        return;
    i = 0;
    while (i < 4)
    {
        if (data->textures[i].img_ptr)
            mlx_destroy_image(data->mlx, data->textures[i].img_ptr);
        i++;
    }
    if (data->img.img_ptr)
        mlx_destroy_image(data->mlx, data->img.img_ptr);
    if (data->win)
        mlx_destroy_window(data->mlx, data->win);
    if (data->mlx)
        mlx_destroy_display(data->mlx);
    free(data->mlx);
    if (data->map)
        clean_map(data->map);
}

t_map   *copy_map(t_map *original)
{
	t_map	*copy;
	int		i;

    copy = malloc(sizeof(t_map));
    if (!copy)
        return (NULL);
    copy->width = original->width;
    copy->height = original->height;
    copy->textures.no = NULL;
    copy->textures.so = NULL;
    copy->textures.we = NULL;
    copy->textures.ea = NULL;
    copy->grid = malloc(sizeof(char *) * (copy->height + 1));
    if (!copy->grid)
    {
        free(copy);
        return (NULL);
    }
    i = 0;
    while (i < copy->height)
    {
        copy->grid[i] = ft_strdup(original->grid[i]);
        i++;
    }
    copy->grid[copy->height] = NULL;
    return (copy);
}
