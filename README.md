*This project has been created as part of the 42 curriculum by seilkiv, cmanuel-.*

# cub3D

## Description

This projects aims to create a Wolfenstein 3D-style raycasting engine built in C using MiniLibX.
It's a real-time 3D maze renderer using raycasting — the same technique used in the **Wolfenstein 3D** (1992). 
The world starts as a 2D grid, and the engine casts rays from the player's viewpoint to determine wall distances, creating a pseudo-3D perspective (raycast).

## Instructions

### Compiling and Building cub3d

```bash
make          # Build project (auto-clones minilibx-linux if missing)
make mlx      # Clones minilibx on demand
make re       # Clean rebuild
make clean    # Remove object files
make fclean   # Remove object files and executable file
make ffclean  # Removes object files, executable and minilibx library
```

### Running

```bash
./cub3D maps/*.cub
```
- `maps` directory has a few maps available to test

### Controls

- use `WS` to move forward/backward
- use `AD` to strafe left/right
- use `<left>` and `<right>` to rotate accordingly (or `h` and `l`, if you're a vim user!)
- `Esc` to close the program

## AI Usage

TODO

## Resources

- [Lode's Raycasting Tutorial](https://lodev.org/cgtutor/raycasting.html) — The **DEFINITIVE RAYCASTING GUIDE!**
- [3DSage Youtube Channel Videos on "Make Your Own Raycaster"](https://www.youtube.com/watch?v=gYRrGTC7GtA)
- [MiniLibX Documentation](https://harm-smits.github.io/42docs/libs/minilibx/getting_started.html)
- [Jun Han Ng - cub3d tutorial](https://hackmd.io/@nszl/H1LXByIE2)

