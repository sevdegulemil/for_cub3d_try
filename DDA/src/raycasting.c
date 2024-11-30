#include <mlx.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MAP_WIDTH 24
#define MAP_HEIGHT 24

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
double posX = 22, posY = 12;
double dirX = -1, dirY = 0;
double planeX = 0, planeY = 0.66;

int exit_program(void)
{
    exit(0);
    return 0;
}

int main()
{
    mlx = mlx_init();
    win = mlx_new_window(mlx, SCREEN_WIDTH, SCREEN_HEIGHT, "raycasting");
    img = mlx_new_image(mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    int bpp, size_line, endian;
    data = mlx_get_data_addr(img, &bpp, &size_line, &endian);

    while (1)
    {
        for (int x = 0; x < SCREEN_WIDTH; x++)
        {
            double cameraX = 2 * x / (double)SCREEN_WIDTH - 1;
            double rayDirX = dirX + planeX * cameraX;
            double rayDirY = dirY + planeY * cameraX;

            int mapX = (int)posX;
            int mapY = (int)posY;

            double sideDistX;
            double sideDistY;

            double deltaDistX = fabs(1 / rayDirX);
            double deltaDistY = fabs(1 / rayDirY);
            double perpWallDist;

            int stepX;
            int stepY;

            stepX = 0;
            stepY = 0;

            int hit = 0;
            int side;

            if (rayDirX < 0)
            {
                stepX = -1;
                sideDistX = (mapX - posX) * deltaDistX;
            }
            else
            {
                stepX = 1;
                sideDistX = (22 + 1.0 - mapX) * deltaDistX;
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
                if (worldMap[mapX][mapY] > 0) hit = 1;
            }

            // if (side == 0)
            //     perpWallDist = (sideDistX - deltaDistX);
            // else
            //     perpWallDist = (sideDistY - deltaDistY);
                 if (side == 0) perpWallDist = (mapX - posX + (1 - stepX) / 2) / rayDirX;
    else perpWallDist = (mapY - posY + (1 - stepY) / 2) / rayDirY;

            int lineHeight = (int)(SCREEN_HEIGHT / perpWallDist);
            int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
            if (drawStart < 0)
                drawStart = 0;
            int drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
            if (drawEnd >= SCREEN_HEIGHT)
                drawEnd = SCREEN_HEIGHT - 1;

            // int color = (worldMap[mapX][mapY] == 1) ? 0xFF0000 : 0x00FF00;
            int color = 0xFF0000;
            if (side == 1)
                color = color / 2;
            for (int y = drawStart; y < drawEnd; y++)
            {
                int pixelIndex = (y * SCREEN_WIDTH + x) * 4;
                data[pixelIndex + 0] = (unsigned char)180;
                data[pixelIndex + 1] = 100;
                data[pixelIndex + 2] = 56;
                data[pixelIndex + 3] = 0;
            }
            int i = 0;
            int j;
            while (i < SCREEN_HEIGHT / 2)
            {
                j = 0;
                while (j < SCREEN_WIDTH)
                {
                    int pixelIndex = (i * SCREEN_WIDTH + j) * 4;
                    data[pixelIndex + 0] = (unsigned char)120;
                    data[pixelIndex + 1] = (unsigned char)230;
                    data[pixelIndex + 2] = (unsigned char)255;
                    data[pixelIndex + 3] = 0;
                    j++;
                }
                i++;
            }
            while (i < SCREEN_HEIGHT)
            {
                j = 0;
                while (j < SCREEN_WIDTH)
                {
                    int pixelIndex = (i * SCREEN_WIDTH + j) * 4;
                    data[pixelIndex + 0] = (unsigned char)160;
                    data[pixelIndex + 1] = (unsigned char)45;
                    data[pixelIndex + 2] = (unsigned char)98;
                    data[pixelIndex + 3] = 0;
                    j++;
                }
                i++;
            }

        }

    mlx_put_image_to_window(mlx, win, img, 0, 0);
    mlx_loop(mlx);
    }

    return 0;
}