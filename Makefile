NAME = cub3D
MLX = minilibx-linux

CC = cc
CFLAGS = -Wall -Wextra -Werror -Iinclude -I$(MLX)
MLXFLAGS = -L$(MLX) -lmlx -lXext -lX11 -lm -lz

SRCS = src/main.c \
		src/utils.c

OBJS = $(SRCS:.c=.o)

all:$(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(MLXFLAGS)
	@echo "Cub3D has been compiled successfully!"

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)
	@echo "Cub3D has been removed successfully!"

re: fclean all

mlx:
	git clone https://github.com/42Paris/minilibx-linux.git $(MLX)
	$(MAKE) -C $(MLX)

.PHONY: all clean fclean re