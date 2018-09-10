/**
 * @brief convert yuv to 264.
 * 
 * @file yuv2x264.cpp
 * @author chengshuyi
 * @date 2018-09-08
 */
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdint.h"

#include "debug.h"
#include "config.h"
#include "yuv2x264.h"
#include "x264.h"

int width, height;
x264_param_t param;
x264_picture_t pic;
x264_picture_t pic_out;
x264_t *h;
int i_frame;
int i_frame_size;
x264_nal_t *nal;
int i_nal;
int luma_size;
int chroma_size;

int init_param_for_x264()
{
    int ret;
    ret = 0;
    width = FRAME_PIX_WIDTH;
    height = FRAME_PIX_HEIGHT;
    i_frame = 0;
    if (x264_param_default_preset(&param, "fast", "zerolatency") < 0)
    {
        debug_print("param preset fault.\n");
    }
    param.i_bitdepth = 8;
    param.i_csp = X264_CSP_I420;
    param.i_width = width;
    param.i_height = height;
    //param.b_vfr_input = 0;
    param.b_repeat_headers = 1;
    param.b_annexb = 1;
    param.i_fps_den  = 1; 
	param.i_fps_num  = FPS;
	param.i_timebase_den = param.i_fps_num;
	param.i_timebase_num = param.i_fps_den;
    if (x264_param_apply_profile(&param, "high") < 0)
    {
        ret = -1;
        debug_print("param apply profile fault.\n");
        return ret;
    }
    //x264_picture_init(&pic);
    if (x264_picture_alloc(&pic, param.i_csp, param.i_width, param.i_height) < 0)
    {
        ret = -1;
        debug_print("alloc memory fault.\n");
        return ret;
    }
    
    h = x264_encoder_open(&param);
    if (!h)
    {
        ret = -1;
        debug_print("init debug handler fault.\n");
        return ret;
    }
    luma_size = width * height;
    chroma_size = luma_size / 4;
    return ret;
}

int encode_one_frame(uint8_t *yuv_img)
{
    int ret;
    debug_print("function encode_one_frame");
    memcpy(pic.img.plane[0], yuv_img, luma_size);
    memcpy(pic.img.plane[1], yuv_img + luma_size, chroma_size);
    memcpy(pic.img.plane[2], yuv_img + luma_size + chroma_size, chroma_size);
    /*
    pic.img.plane[0] = yuv_img;
    pic.img.plane[1] = yuv_img + luma_size;
    pic.img.plane[2] = yuv_img + luma_size + chroma_size;
    */
    pic.i_pts = i_frame;
    ++i_frame;
    i_frame_size = x264_encoder_encode(h, &nal, &i_nal, &pic, &pic_out);
    ret = i_frame_size;
    return ret;
}

void close_yuv2x264()
{
    x264_encoder_close(h);
    x264_picture_clean(&pic);
}