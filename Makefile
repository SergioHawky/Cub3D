NAME = cub3D
MLX = minilibx-linux

CC = cc
CFLAGS = -Wall -Wextra -Werror -Iinclude -I$(MLX)
MLXFLAGS = -L$(MLX) -lmlx -lXext -lX11 -lm -lz

GREEN	:= \033[92m
OFF		:= \033[0m
YELLOW  := \033[33m

SRCS = src/main.c \
	   src/parsing.c \
	   src/map_checker_1.c \
	   src/map_checker_2.c \
	   src/utils.c \
	   src/player.c \
	   src/events.c \
	   src/minimap.c \
	   src/raycast.c \
	   src/key_handler.c

OBJS = $(SRCS:.c=.o)

all: mlx libft.a $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -Llibft -lft $(MLXFLAGS) -o $(NAME)
	@echo "\n$(GREEN)'$(NAME)' compiled successfully!$(OFF)\n"

mlx:
	@if [ ! -d $(MLX) ]; then \
		git clone https://github.com/42Paris/minilibx-linux.git; \
		make -C $(MLX); \
	fi

libft.a:
	@make -s -C libft

clean:
	@make clean -C libft
	rm -f $(OBJS)
	@echo "$(YELLOW)Removed obj files...$(OFF)"

fclean: clean
	@make fclean -C libft
	rm -f $(OBJS)
	rm -f $(NAME)
	@echo "$(YELLOW)Removed $(NAME) executable...$(OFF)"
	@echo "Cub3D has been removed successfully!"

re: fclean all

ffclean: clean fclean
	rm -rf $(MLX)
	@echo "$(YELLOW)Removed minilibx library!$(OFF)"

vc:
	valgrind --leak-check=full --show-leak-kinds=all ./cub3D maps/map1.cub
va:
	valgrind --leak-check=full --show-leak-kinds=all ./cub3D maps/a.cub
vs:
	valgrind --leak-check=full --show-leak-kinds=all ./cub3D maps/s.cub
vd:
	valgrind --leak-check=full --show-leak-kinds=all ./cub3D maps/d.cub
ve:
	valgrind --leak-check=full --show-leak-kinds=all ./cub3D maps/e.cub

cc:
	./cub3D maps/map1.cub
ca:
	./cub3D maps/a.cub
cs:
	./cub3D maps/s.cub
cd:
	./cub3D maps/d.cub


.PHONY: all clean fclean re ffclean vc mlx
