/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_texture.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 17:13:46 by seilkiv           #+#    #+#             */
/*   Updated: 2026/04/09 17:14:46 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

t_img	*get_wall_texture(t_data *data, t_ray *ray)
{
	if (ray->side == 0)
	{
		if (ray->step_x > 0)
			return (&data->textures[3]);
		return (&data->textures[2]);
	}
	if (ray->step_y > 0)
		return (&data->textures[1]);
	return (&data->textures[0]);
}

int	get_texture_pixel(t_img *texture, int x, int y)
{
	char	*pixel;

	if (!texture || !texture->addr)
		return (0);
	if (x < 0)
		x = 0;
	if (y < 0)
		y = 0;
	if (x >= texture->width)
		x = texture->width - 1;
	if (y >= texture->height)
		y = texture->height - 1;
	pixel = texture->addr + (y * texture->line_len) + (x * (texture->bpp / 8));
	return (*(unsigned int *)pixel);
}
