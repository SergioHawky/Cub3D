#ifndef CUB3D_H
#define CUB3D_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mlx.h>

typedef struct s_data
{
    void    *mlx;
    void    *win;
}   t_data;

void    print_message(char *message);
#endif