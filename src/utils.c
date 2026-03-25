/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 utils.c											:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: seilkiv <seilkiv@student.42lisboa.com>		+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2026/03/05 11:34:13 by seilkiv		   #+#	  #+#			  */
/*	 Updated: 2026/03/07 14:28:29 by seilkiv		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "cub3D.h"

void	clean_map(t_map *map)
{
	int	i;

	if (!map)
		return;
	if (map->grid)
	{
		for (i = 0; i < map->height; i++)
			free(map->grid[i]);
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

t_map	*copy_map(t_map *original)
{
	t_map	*copy;
	int		i;

	copy = malloc(sizeof(t_map));
	if (!copy)
		return (NULL);
	copy->width = original->width;
	copy->height = original->height;
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
