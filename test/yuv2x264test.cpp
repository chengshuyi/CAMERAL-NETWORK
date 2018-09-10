#include "stdlib.h"
#include "stdio.h"
#include "stdint.h"

#include "yuv2x264.h"
#include "config.h"

void yuv_x264_test(char *FILE_SRC,char *FILE_DST)
{
    int frames;
    uint8_t *yuv_img;
    int ret;

    FILE *f_src = fopen(FILE_SRC, "rb");
    FILE *f_dst = fopen(FILE_DST, "wb");
    yuv_img = (uint8_t *)malloc(sizeof(uint8_t) * YUV_SIZE);
    init_param_for_x264();

    for (frames = 0;; ++frames)
    {
        if (fread(yuv_img, 1, YUV_SIZE, f_src) != YUV_SIZE)
        {
            printf("Encoding finished!\n");
            break;
        }
        ret = encode_one_frame(yuv_img);
        if (ret < 0)
        {
            printf("wrong encode %d.\n",ret);
            break;
        }
        printf("Encoding %5dth frame and get %d bytes.\n", frames,ret);
        fwrite(nal->p_payload, ret, 1, f_dst);
    }

    close_yuv2x264();
    fcloseall();
}