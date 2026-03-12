#ifndef CUB3D_H
#define CUB3D_H

#include "../libft/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <math.h>
# include <unistd.h>
# include <mlx.h>
# include <X11/X.h>
# include <X11/keysym.h>

# define PX 32
# define WIDTH 1024
# define HEIGHT 512 

# define ERROR_INVALID \
    "ERROR: Invalid number of arguments\n\
    Usage: './cub3d maps/<any_map>.cub'\n"
# define ERROR_PARSING "Map parsing failed\n"
# define ERROR_MLX_INIT "MLX init failed\n"
# define ERROR_MLX_WIN "Window creation failed\n"

// COLORS
# define BLACK 0x000000
# define WHITE 0xFFFFFF
# define UBUNTU 0x5E2750
# define PINK 0xFF69B4

typedef struct 	s_img
{
    void    *img_ptr;
    char    *addr; // addr
    int	    bpp;
    int	    endian;
    int	    line_len;
}		        t_img;

typedef struct 	s_map
{
    char **grid;
    int width;
    int height;
    float player_initial_x;
    float player_initial_y;
    char player_initial_dir;
    int exit_x;
    int exit_y;
}               t_map;

typedef struct 	s_player
{
    float x;
    float y;
    float dir_x;
    float dir_y;
    float move_speed;
    float rot_speed;
}               t_player;

typedef struct s_data
{
    void    *mlx;
    void    *win;
    t_map   *map;
    t_player player;
    t_img	img;
}               t_data;

void    print_message_and_free(char *message, t_data *data, int fd);
int     check_if_map_is_valid(t_map *map, char *filename);
t_map   *parse_map(char *filename);
void    clean_map(t_map *map);
t_map   *copy_map(t_map *original); int     check_borders(t_map *map);
void	events_init(t_data *data);
void 	game_render(t_data *data);
void    draw_player(t_data *data);
void    ft_pixel_put(int x, int y, t_img *img, int color);
void    init_player(t_data *data);
void    rotate_player(t_data *data, float angle);
void    move_player(t_data *data, float forward, float strafe);

#endif
