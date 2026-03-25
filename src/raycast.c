
#include "cub3D.h"

void	init_ray(t_data *data, t_ray *ray, float camera_x)
{
	ray->dir_x = data->player.dir_x + data->player.plane_x * camera_x;
	ray->dir_y = data->player.dir_y + data->player.plane_y * camera_x;
	ray->map_x = (int)data->player.x;
	ray->map_y = (int)data->player.y;
	ray->delta_x = fabsf(ray->dir_y);
	ray->delta_y = fabsf(ray->dir_x);
	ray->step_x = (ray->dir_x < 0) ? -1 : 1;
	if (ray->dir_x < 0)
		ray->side_x = (data->player.x - ray->map_x) * ray->delta_x;
	else
		ray->side_x = (ray->map_x + 1.0f - data->player.x) * ray->delta_x;
	ray->step_y = (ray->dir_y < 0) ? -1 : 1;
	if (ray->dir_y < 0)
		ray->side_y = (data->player.y - ray->map_y) * ray->delta_y;
	else
		ray->side_y = (ray->map_y + 1.0f - data->player.y) * ray->delta_y;
}

void	dda(t_data *data, t_ray *ray)
{
	int	hit;

	hit = 0;
	while (!hit)
	{
		if (ray->side_x < ray->side_y)
		{
			ray->side_x += ray->delta_x;
			ray->map_x += ray->step_x;
			ray->side = 0;
		}
		else
		{
			ray->side_y += ray->delta_y;
			ray->map_y += ray->step_y;
			ray->side = 1;
		}
		if (ray->map_x < 0 || ray->map_y < 0
			|| ray->map_y >= data->map->height
			|| ray->map_x >= data->map->width)
			hit = 1;
		else if (!data->map->grid[ray->map_y][ray->map_x]
			|| data->map->grid[ray->map_y][ray->map_x] == '1')
			hit = 1;
	}
}

static void draw_ray_line(t_data *data, int x0, int y0, int x1, int y1)
{
    float   dx;
    float   dy;
    float   steps;
    int     i;
    int     px;
    int     py;

    dx = x1 - x0;
    dy = y1 - y0;
    steps = fmaxf(fabsf(dx), fabsf(dy));
    if (steps == 0)
        return ;
    i = 0;
    while (i <= (int)steps)
    {
        px = x0 + (int)(dx * i / steps);
        py = y0 + (int)(dy * i / steps);
        if (px >= 0 && px < WIDTH && py >= 0 && py < HEIGHT)
            ft_pixel_put(px, py, &data->img, YELLOW);
        i++;
    }
}

void	cast_rays(t_data *data)
{
	t_ray	ray;
	float	t;
	int		n;
	int		cx;
	int		cy;

	cx = MM_OFF_X + (int)(data->player.x * MM_TILE);
	cy = MM_OFF_Y + (int)(data->player.y * MM_TILE);
	n = 0;
	while (n < NUM_RAYS)
	{
		init_ray(data, &ray, 2.0f * n / (float)(NUM_RAYS - 1) - 1.0f);
		dda(data, &ray);
		if (ray.side == 0)
			t = (ray.map_x - data->player.x + (1 - ray.step_x) / 2.0f) / ray.dir_x;
		else
			t = (ray.map_y - data->player.y + (1 - ray.step_y) / 2.0f) / ray.dir_y;
		draw_ray_line(data, cx, cy,
			MM_OFF_X + (int)((data->player.x + ray.dir_x * t) * MM_TILE),
			MM_OFF_Y + (int)((data->player.y + ray.dir_y * t) * MM_TILE));
		n++;
	}
}
    
