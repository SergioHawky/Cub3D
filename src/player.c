/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 15:35:28 by seilkiv           #+#    #+#             */
/*   Updated: 2026/04/15 23:40:13 by cmanuel-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static void	draw_circle(t_data *data, int cx, int cy, int r)
{
	int	dx;
	int	dy;

	dy = -r;
	while (dy <= r)
	{
		dx = -r;
		while (dx <= r)
		{
			if (dx * dx + dy * dy <= r * r)
				ft_pixel_put(cx + dx, cy + dy, &data->img, PINK);
			dx++;
		}
		dy++;
	}
}

void	init_player(t_data *data)
{
	char	dir;

	data->player.x = data->map->player_initial_x + 0.5f;
	data->player.y = data->map->player_initial_y + 0.5f;
	data->player.move_speed = SPEED;
	data->player.rot_speed = ROT_SPEED;
	dir = data->map->player_initial_dir;
	data->player.dir_x = (float)((dir == 'E') - (dir == 'W'));
	data->player.dir_y = (float)((dir == 'S') - (dir == 'N'));
	data->player.plane_x = ((dir == 'N') - (dir == 'S')) * PLANE_LEN;
	data->player.plane_y = ((dir == 'E') - (dir == 'W')) * PLANE_LEN;
}

void	move_player(t_data *data, float forward, float strafe)
{
	float	move_x;
	float	move_y;

	move_x = 0.0f;
	move_y = 0.0f;
	move_x += data->player.dir_x * data->player.move_speed * forward;
	move_y += data->player.dir_y * data->player.move_speed * forward;
	move_x += -data->player.dir_y * data->player.move_speed * strafe;
	move_y += data->player.dir_x * data->player.move_speed * strafe;
	if (!collision(data, data->player.x + move_x, data->player.y))
		data->player.x += move_x;
	if (!collision(data, data->player.x, data->player.y + move_y))
		data->player.y += move_y;
}

void	rotate_player(t_data *data, float angle)
{
	float	old_dir_x;
	float	old_plane_x;
	float	cos_a;
	float	sin_a;

	old_dir_x = data->player.dir_x;
	old_plane_x = data->player.plane_x;
	cos_a = cosf(angle);
	sin_a = sinf(angle);
	data->player.dir_x = data->player.dir_x * cos_a - data->player.dir_y
		* sin_a;
	data->player.dir_y = old_dir_x * sin_a + data->player.dir_y * cos_a;
	data->player.plane_x = data->player.plane_x * cos_a - data->player.plane_y
		* sin_a;
	data->player.plane_y = old_plane_x * sin_a + data->player.plane_y * cos_a;
}

void	draw_player(t_data *data)
{
	t_minimap	mm;
	int			cx;
	int			cy;
	int			r;

	minimap_get_layout(data, &mm);
	r = mm.tile_px / 3;
	if (r < 2)
		r = 2;
	cx = mm.off_x + (int)(data->player.x * mm.tile_px);
	cy = mm.off_y + (int)(data->player.y * mm.tile_px);
	draw_circle(data, cx, cy, r);
}
