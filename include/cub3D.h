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

#define MINIMAP 1

// MACROS
# define PX 32
# define WIDTH 1024
# define HEIGHT 512 
# define SPEED 0.10f 
# define ROT_SPEED 0.01f
# define MINIMAP_SIZE 220
# define MINIMAP_MARGIN 10
# define MINIMAP_MIN_TILE 4
# define MAX_VISIBLE_MAP_SIDE (MINIMAP_SIZE / MINIMAP_MIN_TILE)
# define PI 3.14159265358979323846f
# define FOV (60.0f * PI / 180.0f)      //graus * PI / 180
# define PLANE_LEN tanf(FOV * 0.5f)

// ERROR MESSAGES
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
# define YELLOW 0xFFEA00
# define RED 0xDC143C
# define GREY 0xB2BEB5        // light grey
# define CHARCOAL 0x36454F    // dark grey

typedef struct 	s_img
{
    void    *img_ptr;
    char    *addr; // addr
    int	    bpp;
    int	    endian;
    int	    line_len;
    int     width;
    int     height;
}		        t_img;

typedef struct s_textures
{
    char    *no;
    char    *so;
    char    *we;
    char    *ea;
}               t_textures;

typedef struct 	s_map
{
    char **grid;
    int width;
    int height;
    int floor_color;
    int ceiling_color;
    float player_initial_x;
    float player_initial_y;
    char player_initial_dir;
    int exit_x;
    int exit_y;
    t_textures textures;
}               t_map;

typedef struct 	s_player
{
    float x;
    float y;
    float dir_x;
    float dir_y;
    float plane_x;
    float plane_y;
    float move_speed;
    float rot_speed;
}               t_player;

typedef struct s_ray
{
    float   ray_dir_x;
    float   ray_dir_y;
    int     map_x;        // tile atual do raio
    int     map_y;
    float   delta_dist_x; // distância entre cruzamentos de grid em X
    float   delta_dist_y;
    float   side_dist_x;  // distância até ao próximo cruzamento X
    float   side_dist_y;
    int     step_x;       // direção de avanço (-1 ou +1)
    int     step_y;
    int     side;         // 0 = bateu em parede N/S, 1 = parede E/W
    float   perp_dist;    // distância perpendicular (sem fisheye)
}   t_ray;

typedef enum e_key
{
    KEY_W = 0,
    KEY_S,
    KEY_A,
    KEY_D,
    KEY_LEFT,
    KEY_RIGHT,
    KEY_COUNT
}               t_key;

typedef struct s_data
{
    void        *mlx;
    void        *win;
    t_map       *map;
    t_player    player;
    t_img	    img;
    t_img       textures[4];
    int         keys[KEY_COUNT];
}               t_data;

typedef struct s_minimap
{
    int	off_x;
    int	off_y;
    int	tile_px;
    int	mini_w;
    int	mini_h;
}	t_minimap;

void    print_message_and_free(char *message, t_data *data, int fd);
void    cleanup_data(t_data *data);
int     check_if_map_is_valid(t_map *map, char *filename);
t_map   *parse_map(char *filename);
void    clean_map(t_map *map);
t_map   *copy_map(t_map *original); 
int     check_borders(t_map *map);
void	events_init(t_data *data);
int     game_loop(t_data *data);
void 	game_render(t_data *data);
void    draw_player(t_data *data);
void    ft_pixel_put(int x, int y, t_img *img, int color);
void    init_player(t_data *data);
void    rotate_player(t_data *data, float angle);
void    move_player(t_data *data, float forward, float strafe);
int     collision(t_data *data, float new_x, float new_y);
void    draw_minimap(t_data *data);
void    minimap_get_layout(t_data *data, t_minimap *mm);
int     is_wall(t_data *data, float x, float y);
void    draw_ray_minimap(t_data *data, t_ray *ray, int x);
void    render_3d(t_data *data);
void    render_minimap_rays(t_data *data);
int     key_press_handler(int keysym, t_data *data);
int     key_release_handler(int keysym, t_data *data);
int     close_handler(t_data *data);
int     init_data(char **argv, t_data *data);


t_img   *get_wall_texture(t_data *data, t_ray *ray);
int     get_texture_pixel(t_img *texture, int x, int y);
int     is_blank_char(char c);
void    strip_line_end(char *line);
char    *trim_spaces(char *str);
void    free_split(char **parts);
int     is_only_digits(char *str);
int     is_map_line(char *line);
int     is_color_line(char *line);
int     parse_texture_line(t_map *map, char *line);
int     parse_color_line(t_map *map, char *line);

#endif
