#include "stdlib.h"
#include "stdio.h"
#include "stdint.h"
#include "unistd.h"

#include "yuv2x264.h"
#include "config.h"
#include "transfer.h"

void transfer_test()
{
    int frames;
    char FILE_SRC[] = ".././testdir/akiyo_cif.yuv";
    char FILE_DST[] = ".././testdir/akiyo_cif.264";
    uint8_t *yuv_img;
    int ret;
    int i;
    uint8_t dst_ip[] = {127, 0, 0, 1};

    ret = 1;
    FILE *f_src = fopen(FILE_SRC, "rb");
    FILE *f_dst = fopen(FILE_DST, "wb");
    yuv_img = (uint8_t *)malloc(sizeof(uint8_t) * YUV_SIZE);
    ret = init_param_for_x264();
    if (ret < 0)
    {
        printf("ret = %d , init param for x264 caused fault.\n", ret);
        goto FAIL;
    }
    ret = init_param_for_jrtplib();
    if (ret < 0)
    {
        printf("ret = %d , init param for jrtp caused fault.\n", ret);
        goto FAIL;
    }
    for (frames = 0;; ++frames)
    {
        if (fread(yuv_img, 1, YUV_SIZE, f_src) != YUV_SIZE)
        {
            printf("Encoding finished!\n");
            break;
        }
        /**
         * @brief 因为config里面有定义，需要把之前make的给clean，重新编译出.o文件
         * 
         */
        ret = encode_one_frame(yuv_img);
        if (ret < 0)
        {
            printf("wrong encode %d.\n", ret);
            break;
        }
        printf("Encoding %5dth frame and get %d bytes and get %d nalus.\n", frames, ret, i_nal);
        fwrite(nal->p_payload, ret, 1, f_dst);
        /*
        for (i = 0; i < i_nal; ++i)
        {
            printf("transfer %dth nalu  size = %d\n",i,nal[i].i_payload);
            ret = transfer_one_nalu_by_rtp(nal[i].p_payload, nal[i].i_payload, i==0?1:0);
            if (ret < 0)
            {
                printf("ret = %d , init param for x264 caused fault.\n", ret);
                goto FAIL;
            }
            usleep(30*1000);
        }
        */
    }
FAIL:
    close_yuv2x264();
    fcloseall();
}