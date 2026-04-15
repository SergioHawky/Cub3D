/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 15:35:24 by seilkiv           #+#    #+#             */
/*   Updated: 2026/04/13 17:25:11 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	minimap_get_layout(t_data *data, t_minimap *mm)
{
	int	tpx_w;
	int	tpx_h;

	tpx_w = MINIMAP_SIZE / data->map->width;
	tpx_h = MINIMAP_SIZE / data->map->height;
	mm->tile_px = tpx_w;
	if (tpx_h < mm->tile_px)
		mm->tile_px = tpx_h;
	if (mm->tile_px < 1)
		mm->tile_px = 1;
	mm->mini_w = data->map->width * mm->tile_px;
	mm->mini_h = data->map->height * mm->tile_px;
	mm->off_x = MINIMAP_MARGIN;
	mm->off_y = HEIGHT - mm->mini_h - MINIMAP_MARGIN;
	if (mm->off_y < 0)
		mm->off_y = 0;
}

static void	draw_tile(t_data *data, int x, int y, int color)
{
	t_minimap	mm;
	int			px;
	int			py;

	minimap_get_layout(data, &mm);
	py = 0;
	while (py < mm.tile_px)
	{
		px = 0;
		while (px < mm.tile_px)
		{
			ft_pixel_put(mm.off_x + x * mm.tile_px + px,
				mm.off_y + y * mm.tile_px + py, &data->img, color);
			px++;
		}
		py++;
	}
}

static int	tile_color(t_data *data, int map_x, int map_y)
{
	char	c;

	if (map_x < 0 || map_y < 0 || map_y >= data->map->height
		|| map_x >= data->map->width)
		return (BLACK);
	c = data->map->grid[map_y][map_x];
	if (c == '1')
		return (WHITE);
	if (c == '0' || c == 'N' || c == 'S' || c == 'E' || c == 'W' || c == 'A')
		return (UBUNTU);
	return (BLACK);
}

void	draw_minimap(t_data *data)
{
	int	i;
	int	j;

	i = 0;
	while (i < data->map->height)
	{
		j = 0;
		while (j < data->map->width)
		{
			draw_tile(data, j, i, tile_color(data, j, i));
			j++;
		}
		i++;
	}
	render_minimap_rays(data);
	draw_player(data);
}
