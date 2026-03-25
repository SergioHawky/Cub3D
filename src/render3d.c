
#include "cub3D.h"

static void	draw_column(t_data *data, int x, float perp, int side)
{
	int	wall_h;
	int	start;
	int	end;
	int	y;
	int	color;

	wall_h = (int)(HEIGHT / perp);
	start = HEIGHT / 2 - wall_h / 2;
	end = HEIGHT / 2 + wall_h / 2;
	if (start < 0)
		start = 0;
	if (end >= HEIGHT)
		end = HEIGHT - 1;
	color = (side == 0) ? WHITE : 0xAAAAAA;
	y = 0;
	while (y < start)
		ft_pixel_put(x, y++, &data->img, CHARCOAL);
	while (y <= end)
		ft_pixel_put(x, y++, &data->img, color);
	while (y < HEIGHT)
		ft_pixel_put(x, y++, &data->img, GREY);
}

void	render_3d(t_data *data)
{
	t_ray	ray;
	float	perp;
	int		x;

	x = 0;
	while (x < WIDTH)
	{
		init_ray(data, &ray, 2.0f * x / WIDTH - 1.0f);
		dda(data, &ray);
		if (ray.side == 0)
			perp = (ray.map_x - data->player.x + (1 - ray.step_x) / 2.0f) / ray.dir_x;
		else
			perp = (ray.map_y - data->player.y + (1 - ray.step_y) / 2.0f) / ray.dir_y;
		draw_column(data, x, perp, ray.side);
		x++;
	}
}
