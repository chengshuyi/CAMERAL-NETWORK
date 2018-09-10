/**
 * @brief the reference of how to compile x264 in linux. https://blog.csdn.net/tuolaji8/article/details/51277767
 */

#ifndef YUV2X264_H
#define YUV2X264_H

#if defined(__cplusplus)
extern "C"
{
#include "x264.h"
};
#else
#include "x264.h"
#endif
#include "x264_config.h"

extern x264_nal_t *nal;
extern int i_nal;

/**
 * @brief init the x264 params.
 * 
 * @return int if -1 , something is wrong.
 */
int init_param_for_x264();

/**
 * @brief encode one frame.
 * 
 * @param yuv_img char pointer which points to the yuv image store memory start.
 * @return int the length of respect h264
 */
int encode_one_frame(uint8_t *yuv_img);
void close_yuv2x264();

#endif
