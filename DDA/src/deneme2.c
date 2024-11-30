#include <mlx.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MAP_WIDTH 24
#define MAP_HEIGHT 24

// Key codes for movement and actions
#define KEY_W 13
#define KEY_A 0
#define KEY_S 1
#define KEY_D 2
#define KEY_LEFT 123
#define KEY_RIGHT 124
#define KEY_ESC 53

int worldMap[MAP_WIDTH][MAP_HEIGHT] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
    {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
    {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

void *mlx;
void *win;
void *img;
char *data;

int w = SCREEN_WIDTH;
int h = SCREEN_HEIGHT;
double posX = 22, posY = 12; // Initial position
double dirX = -1, dirY = 0; // Initial direction
double planeX = 0, planeY = 0.66; // Camera plane

// Exit program
int exit_program(void)
{
    exit(0);
    return 0;
}

// Update player position and handle key presses
void movePlayer(int keycode)
{
    double moveSpeed = 0.1;
    double rotSpeed = 0.05;

    if (keycode == KEY_W)
    {
        if (worldMap[(int)(posX + dirX * moveSpeed)][(int)posY] == 0)
            posX += dirX * moveSpeed;
        if (worldMap[(int)posX][(int)(posY + dirY * moveSpeed)] == 0)
            posY += dirY * moveSpeed;
    }
    if (keycode == KEY_S)
    {
        if (worldMap[(int)(posX - dirX * moveSpeed)][(int)posY] == 0)
            posX -= dirX * moveSpeed;
        if (worldMap[(int)posX][(int)(posY - dirY * moveSpeed)] == 0)
            posY -= dirY * moveSpeed;
    }
    if (keycode == KEY_A)
    {
        if (worldMap[(int)(posX - planeX * moveSpeed)][(int)posY] == 0)
            posX -= planeX * moveSpeed;
        if (worldMap[(int)posX][(int)(posY - planeY * moveSpeed)] == 0)
            posY -= planeY * moveSpeed;
    }
    if (keycode == KEY_D)
    {
        if (worldMap[(int)(posX + planeX * moveSpeed)][(int)posY] == 0)
            posX += planeX * moveSpeed;
        if (worldMap[(int)posX][(int)(posY + planeY * moveSpeed)] == 0)
            posY += planeY * moveSpeed;
    }
    if (keycode == KEY_LEFT)
    {
        double oldDirX = dirX;
        dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
        dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);

        double oldPlaneX = planeX;
        planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
        planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
    }
    if (keycode == KEY_RIGHT)
    {
        double oldDirX = dirX;
        dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
        dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);

        double oldPlaneX = planeX;
        planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
        planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
    }
}

// Key press handler
int keyPress(int keycode)
{
    if (keycode == KEY_ESC)
        exit(0);

    movePlayer(keycode);
    return 0;
}

// Main raycasting loop
int main()
{
    mlx = mlx_init();
    win = mlx_new_window(mlx, SCREEN_WIDTH, SCREEN_HEIGHT, "Raycasting");
    img = mlx_new_image(mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    int bpp, size_line, endian;
    data = mlx_get_data_addr(img, &bpp, &size_line, &endian);

    mlx_hook(win, 2, 1L<<0, keyPress, 0); // Key press event
    mlx_hook(win, 17, 0L, exit_program, 0); // Window close event

    while (1)
    {
        for (int x = 0; x < SCREEN_WIDTH; x++)
        {
            double cameraX = 2 * x / (double)SCREEN_WIDTH - 1;
            double rayDirX = dirX + planeX * cameraX;
            double rayDirY = dirY + planeY * cameraX;

            int mapX = (int)posX;
            int mapY = (int)posY;

            double sideDistX, sideDistY;
            double deltaDistX = fabs(1 / rayDirX);
            double deltaDistY = fabs(1 / rayDirY);
            double perpWallDist;
            int stepX, stepY;

            if (rayDirX < 0)
            {
                stepX = -1;
                sideDistX = (posX - mapX) * deltaDistX;
            }
            else
            {
                stepX = 1;
                sideDistX = (mapX + 1.0 - posX) * deltaDistX;
            }

            if (rayDirY < 0)
            {
                stepY = -1;
                sideDistY = (posY - mapY) * deltaDistY;
            }
            else
            {
                stepY = 1;
                sideDistY = (mapY + 1.0 - posY) * deltaDistY;
            }

            int hit = 0;
            int side = 0;
            while (hit == 0)
            {
                if (sideDistX < sideDistY)
                {
                    sideDistX += deltaDistX;
                    mapX += stepX;
                    side = 0;
                }
                else
                {
                    sideDistY += deltaDistY;
                    mapY += stepY;
                    side = 1;
                }

                if (worldMap[mapX][mapY] > 0)
                    hit = 1;
            }

            if (side == 0)
                perpWallDist = (mapX - posX + (1 - stepX) / 2) / rayDirX;
            else
                perpWallDist = (mapY - posY + (1 - stepY) / 2) / rayDirY;

            int lineHeight = (int)(SCREEN_HEIGHT / perpWallDist);
            int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
            if (drawStart < 0)
                drawStart = 0;
            int drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
            if (drawEnd >= SCREEN_HEIGHT)
                drawEnd = SCREEN_HEIGHT - 1;

            int color = 0x00FF00; // Green for walls
            if (side == 1)
                color = color / 2;

            for (int y = drawStart; y < drawEnd; y++)
            {
                data[y * size_line / 4 + x] = color;
            }
        }
        mlx_put_image_to_window(mlx, win, img, 0, 0);
        mlx_loop(mlx);
    }
}
