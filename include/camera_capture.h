#ifndef CAMERA_CAPTURE_H
#define CAMERA_CAPTURE_H

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <linux/types.h>
#include <linux/videodev2.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include <sys/mman.h>
#include <errno.h>
#include <assert.h>

/**
 * @brief structure for the memory in kernel.
 * 
 */
typedef struct
{
    void *start;
    int length;
} BUFTYPE;

extern BUFTYPE *usr_buf;

/**
 * @brief open camera and choose the correct input interface.
 * 
 * @return int file descriptor.
 */
int open_camera(void);

/**
 * @brief init camera and set relevent parameters and allocate memory.
 * 
 */
void init_camera(int fd);

/**
 * @brief put the buffer into the ready queue of video.
 * 
 * @param fd file descriptor.
 */
void start_capture(int fd);
/**
 * @brief read frame into the buffer.
 * 
 * @param fd 
 */
void read_frame(int fd,uint8_t *img);
/**
 * @brief close camera and free memory.
 * 
 */
void close_camera_device(int fd);

#endif
