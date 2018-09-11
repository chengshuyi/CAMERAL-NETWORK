#include "stdint.h"

#include "config.h"
#include "debug.h"
#include "yuyv2yuv.h"

void yuyv_2_yuv(uint8_t *yuyv_img, uint8_t *yuv_img)
{
    int ynum, total_len;
    int i, j, k;
    uint8_t *y, *u, *v;
    uint8_t switches;
    int width,height;
    width = FRAME_PIX_WIDTH;
    height = FRAME_PIX_HEIGHT;

    debug_print("function yuyv_tran_yuv");

    ynum = width * height;
    total_len = (ynum << 1);
    y = yuv_img;
    u = yuv_img + ynum;
    v = yuv_img + ynum + ynum / 4;

    for (i = 0; i < total_len; i += 2)
    {
        *y++ = yuyv_img[i];
    }

    for (i = 0; i < height; i += 2)
    {
        k = (i * width << 1);
        for (j = 1; j < (width << 1); j += 2)
        {
            if (switches)
            {
                *u++ = ((uint8_t *)(yuyv_img + k))[j];
                switches = 0;
            }
            else
            {
                *v++ = ((uint8_t *)(yuyv_img + k))[j];
                switches = 1;
            }
        }
    }
}