#ifndef YUYV2YUV_H
#define YUYV2YUV_H

/**
 * @brief convert yuyv to yuv(planar).
 * 
 * @param yuyv_img  char pointer which point to yuyv.
 * @param yuv_img   char pointer which point to yuv.
 * @param width     the width of image.
 * @param height    the height of image.
 */
void yuyv_2_yuv(uint8_t *yuyv_img, uint8_t *yuv_img, int width, int height);

#endif
