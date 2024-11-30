#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "../lib/libmlx/mlx.h"

void dda_algorithm(float x1, float y1, float x2, float y2, void *mlx, void *window)
{
    int dx; //x koordinatındaki değişim
    int dy; //y koordinatındaki değişim
    int step; //adım sayısı
    float x_step; //x koordinatındaki adım
    float y_step; //y koordinatındaki adım
    float x_start;
    float y_start;
    int i;

    dx = x2 - x1;
    dy = y2 - y1;
    step = abs(dx) > abs(dy) ? abs(dx) : abs(dy); //dx > dy ise adım sayısı dx olacak, değilse dy olcak (abs ile mutlak değer alıyoruz)
    x_step = dx / (float)step;
    y_step = dy / (float)step;

    x_start = x1;
    y_start = y1;
    i = 0;
    printf("\nÇizilen Noktalar:\n");

    // DDA algoritmasında her adımda piksel piksel çizilcek 
    while (i <= step)
    {
        int x_pixel = (int)round(x_start);
        int y_pixel = (int)round(y_start);

        // Pencere sınırları kontrolü (800x600 pencerede)
        if (x_pixel < 0 || x_pixel > 799 || y_pixel < 0 || y_pixel > 599) 
        {
            printf("Hata: Piksel (%d, %d) pencere dışına çıkıyor\n", x_pixel, y_pixel);
            return;
        }

        // mlx_pixel_put fonksiyonu ile piksel piksel çizim yapılır
        if (mlx_pixel_put(mlx, window, x_pixel, y_pixel, 0xFFFFFF) == -1) 
        {
            printf("Hata: pixel_put hatası (%d, %d)\n", x_pixel, y_pixel);
            return;
        }

        // Çizilen her noktayı ekrana yaz
        printf("(%d , %d)\n", x_pixel, y_pixel);

        x_start += x_step;
        y_start += y_step;
        i++;
    }

    printf("\nDoğruyu çizmek için atılan adım sayısı : %d\n", step);
}

int main()
{
    float x1, y1, x2, y2;
    void *mlx;
    void *window;

    mlx = mlx_init();
    if (mlx == NULL)
    {
        printf("mlx_init hatası\n");
        return (-1);
    }

    window = mlx_new_window(mlx, 800, 600, "DDA Algorithm");
    if (window == NULL)
    {
        printf("Window oluşma hatası\n");
        return (-1);
    }

    printf("\nBaşlangıç değerleri gir (x1, y1) : ");
    if (scanf("%f %f", &x1, &y1) != 2)
    {
        printf("İki sayı girmelisin\n");
        return (-1);
    }

    // Başlangıç noktasının pencere sınırları içinde olup olmadığını kontrol et
    if (x1 < 0 || x1 > 799 || y1 < 0 || y1 > 599) {
        printf("Hata: Başlangıç noktası pencere dışına çıkıyor (%f, %f)\n", x1, y1);
        return (-1);
    }
    printf("Bitiş değerleri gir (x2, y2) : ");
    if (scanf("%f %f", &x2, &y2) != 2)
    {
        printf("İki sayı girmelisin\n");
        return (-1);
    }
    // Bitiş noktasının pencere sınırları içinde olup olmadığını kontrol et
    if (x2 < 0 || x2 > 799 || y2 < 0 || y2 > 599) {
        printf("Hata: Bitiş noktası pencere dışına çıkıyor (%f, %f)\n", x2, y2);
        return (-1);
    }
    dda_algorithm(x1, y1, x2, y2, mlx, window);
    mlx_loop(mlx);

    return (0);
}
