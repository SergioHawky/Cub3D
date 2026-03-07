NAME = cub3D
MLX = minilibx-linux

CC = cc
CFLAGS = -Wall -Wextra -Werror -Iinclude -I$(MLX)
MLXFLAGS = -L$(MLX) -lmlx -lXext -lX11 -lm -lz

SRCS = src/main.c \
	   src/parsing.c \
	   src/map_checker_1.c \
	   src/map_checker_2.c \
	   src/utils.c \
	   src/events.c
	

OBJS = $(SRCS:.c=.o)

all:$(NAME)

$(NAME): $(OBJS)
	$(MAKE) -C libft
	$(CC) $(CFLAGS) $(OBJS) -Llibft -lft $(MLXFLAGS) -o $(NAME)
	@echo "Cub3D has been compiled successfully!"

clean:
	rm -f $(OBJS)
	$(MAKE) -C libft clean

fclean: clean
	$(MAKE) -C libft fclean
	rm -f $(NAME)
	@echo "Cub3D has been removed successfully!"

re: fclean all

mlx:
	git clone https://github.com/42Paris/minilibx-linux.git $(MLX)
	$(MAKE) -C $(MLX)

ffclean: clean fclean
	rm -rf $(MLX)

vc:
	valgrind --leak-check=full --show-leak-kinds=all ./cub3D maps/map1.cub

.PHONY: all clean fclean re
