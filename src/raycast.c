
#include "cub3D.h"

static int ray_hit(t_data *data, int mx, int my)
{
	if (mx < 0 || my < 0 || my >= data->map->height || mx >= data->map->width)
		return (1);
	if (!data->map->grid[my][mx])
		return (1);
	return (data->map->grid[my][mx] == '1');
}

static void set_ray(t_data *data, t_ray *ray)
{
    if (ray->dir_x < 0)
    {
        ray->step_x = -1;
        ray->side_x = (data->player.x - ray->map_x) * ray->delta_x;
    }
    else
    {
        ray->step_x = 1;
        ray->side_x = (ray->map_x + 1.0f - data->player.x) * ray->delta_x;
    }
    if (ray->dir_y < 0)
    {
        ray->step_y = -1;
        ray->side_y = (data->player.y - ray->map_y) * ray->delta_y;
    }
    else
    {
        ray->step_y = 1;
        ray->side_y = (ray->map_y + 1.0f - data->player.y) * ray->delta_y;
    }
}

static void init_ray(t_data *data, t_ray *ray, int n)
{
	float camera_x;

	camera_x = 2.0f * n / (float)(NUM_RAYS - 1) - 1.0f;
	ray->dir_x = data->player.dir_x + data->player.plane_x *camera_x;
	ray->dir_y = data->player.dir_y + data->player.plane_y *camera_x;
	ray->map_x = (int)data->player.x;
	ray->map_y = (int)data->player.y;
	ray->delta_x = fabsf(ray->dir_y);
	ray->delta_y = fabsf(ray->dir_x);
	set_ray(data, ray);
}

static void dda(t_data *data, t_ray *ray)
{
	int hit;

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
		if (ray_hit(data, ray->map_x, ray->map_y))
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

void cast_rays(t_data *data)
{
    t_ray ray;
	float t;
	float hit_x;
	float hit_y;
	int	n;

	n = 0;
	while (n < NUM_RAYS)
	{
		init_ray(data, &ray, n);
		dda(data, &ray);
		if (ray.side == 0)
			t = (ray.map_x - data->player.x + (1 - ray.step_x) / 2.0f) / ray.dir_x;
		else
			t = (ray.map_y - data->player.y + (1 - ray.step_y) / 2.0f) / ray.dir_y;
		hit_x = data->player.x + ray.dir_x * t;
		hit_y = data->player.y + ray.dir_y * t;
		// REDO draw_ray_line(), it has too many variables!
		draw_ray_line(data,
			(int)(data->player.x * PX), (int)(data->player.y * PX),
			(int)(hit_x * PX), (int)(hit_y * PX));
		n++;
	}
}
    
