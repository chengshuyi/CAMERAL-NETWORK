/**
 * @brief test for converting yuyv to yuv.
 * 
 * @file yuyv2yuvtest.cpp
 * @author chengshuyi
 * @date 2018-09-08
 */
#include "stdlib.h"
#include "stdio.h"
#include "stdint.h"

#include "yuyv2yuv.h"
#include "config.h"

void yuyv_yuv_test(char *FILE_SRC,char *FILE_DST)
{
    int frames;
    uint8_t *yuyv_img,*yuv_img;

    yuyv_img = (uint8_t *)malloc(sizeof(uint8_t) * YUYV_SIZE);
    yuv_img = (uint8_t *)malloc(sizeof(uint8_t) * YUV_SIZE);

    FILE *f_src = fopen(FILE_SRC, "rb");
    FILE *f_dst = fopen(FILE_DST, "wb");
    for (frames = 0;; ++frames)
    {
        if (fread(yuyv_img, 1, YUYV_SIZE, f_src) != YUYV_SIZE)
        {
            printf("Encoding finished!\n");
            break;
        }
        yuyv_2_yuv(yuyv_img, yuv_img, FRAME_PIX_WIDTH, FRAME_PIX_HEIGHT);
        fwrite(yuv_img, 1, YUV_SIZE, f_dst);
        printf("Encoding %5dth frame.\n", frames);
    }
    fclose(f_src);
    fclose(f_dst);
    free(yuyv_img);
    free(yuv_img);
}
