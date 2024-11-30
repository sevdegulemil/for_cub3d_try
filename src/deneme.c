#include <mlx.h>
#include <stdlib.h>
#include <math.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MAP_WIDTH 24
#define MAP_HEIGHT 24

// Harita verileri: 1'ler duvarları, 0'lar boş alanları temsil eder
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
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

// Başlangıç oyuncu pozisyonu ve yönü
double posX = 22.0, posY = 12.0;
double dirX = -1.0, dirY = 0.0;
double planeX = 0.0, planeY = 0.66;

void *mlx;
void *win;
void *img;
char *data;

int bpp, size_line, endian;

void put_pixel(int x, int y, int color)
{
    int pixelIndex = (y * SCREEN_WIDTH + x) * 4;
    data[pixelIndex + 0] = (color >> 16) & 0xFF; // Red
    data[pixelIndex + 1] = (color >> 8) & 0xFF;  // Green
    data[pixelIndex + 2] = color & 0xFF;         // Blue
    data[pixelIndex + 3] = 0;                    // Alpha (unused)
}

void draw_background()
{
    // Üst yarıyı mavi, alt yarıyı yeşil çiz
    for (int y = 0; y < SCREEN_HEIGHT / 2; y++) 
    {
        for (int x = 0; x < SCREEN_WIDTH; x++)
        {
            put_pixel(x, y, 0x87CEEB); // Sky Blue
        }
    }
    for (int y = SCREEN_HEIGHT / 2; y < SCREEN_HEIGHT; y++) 
    {
        for (int x = 0; x < SCREEN_WIDTH; x++)
        {
            put_pixel(x, y, 0x228B22); // Forest Green
        }
    }
}

void perform_raycasting()
{
    for (int x = 0; x < SCREEN_WIDTH; x++)
    {
        double cameraX = 2 * x / (double)SCREEN_WIDTH - 1; // Kamera koordinatları
        double rayDirX = dirX + planeX * cameraX;  // Işın yönü X
        double rayDirY = dirY + planeY * cameraX;  // Işın yönü Y

        int mapX = (int)posX;
        int mapY = (int)posY;

        double sideDistX;
        double sideDistY;

        double deltaDistX = fabs(1 / rayDirX);
        double deltaDistY = fabs(1 / rayDirY);
        double perpWallDist;

        int stepX, stepY;
        int hit = 0;
        int side;

        // Başlangıç ray yayılma adımı
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

        // DDA (Digital Differential Analysis) Algoritması ile ışın izleme
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

        // Mesafe hesaplaması
        if (side == 0) perpWallDist = (mapX - posX + (1 - stepX) / 2) / rayDirX;
        else perpWallDist = (mapY - posY + (1 - stepY) / 2) / rayDirY;

        int lineHeight = (int)(SCREEN_HEIGHT / perpWallDist);

        int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
        if (drawStart < 0) drawStart = 0;
        int drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
        if (drawEnd >= SCREEN_HEIGHT) drawEnd = SCREEN_HEIGHT - 1;

        int color = 0xFF0000; // Kırmızı duvar
        if (side == 1) color = color / 2; // Yansıyan yüzeyde rengin koyulaşması

        // Duvar çizimi
        for (int y = drawStart; y < drawEnd; y++)
        {
            put_pixel(x, y, color);
        }
    }
}

int main()
{
    mlx = mlx_init();
    win = mlx_new_window(mlx, SCREEN_WIDTH, SCREEN_HEIGHT, "Raycasting");

    img = mlx_new_image(mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
    data = mlx_get_data_addr(img, &bpp, &size_line, &endian);

    while (1)
    {
        draw_background(); // Arka planı çiz
        perform_raycasting(); // Raycasting ile duvarları çiz

        mlx_put_image_to_window(mlx, win, img, 0, 0); // Görüntüyü pencereye gönder
        mlx_loop(mlx); // Loop
    }

    return 0;
}
