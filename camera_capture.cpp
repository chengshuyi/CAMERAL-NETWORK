/*=============================================================================
#     FileName: v4l2.c
#         Desc: this program aim to get image from USB camera,
#               used the V4L2 interface.
#       Author: Licaibiao
#      Version: 
#   LastChange: 2016-12-10 
#      History:
=============================================================================*/

/**
 * @brief Modify this file.
 * 
 * @file camera_capture.cpp
 * @author chengshuyi
 * @date 2018-09-07
 */

#include "stdint.h"

#include "camera_capture.h"
#include "debug.h"
#include "config.h"

BUFTYPE *usr_buf;
static unsigned int n_buffer = 0;

static void stop_capture(int fd)
{
    enum v4l2_buf_type type;
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (-1 == ioctl(fd, VIDIOC_STREAMOFF, &type))
    {
        debug_print("Fail to ioctl 'VIDIOC_STREAMOFF'");
        exit(EXIT_FAILURE);
    }
}

static void init_mmap(int fd)
{
    struct v4l2_requestbuffers reqbufs;
    memset(&reqbufs, 0, sizeof(reqbufs));
    reqbufs.count = 1;
    reqbufs.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    reqbufs.memory = V4L2_MEMORY_MMAP;
    if (-1 == ioctl(fd, VIDIOC_REQBUFS, &reqbufs))
    {
        debug_print("Fail to ioctl 'VIDIOC_REQBUFS'");
        exit(EXIT_FAILURE);
    }
    n_buffer = reqbufs.count;
    debug_print("the number of buffers: n_buffer = %d\n", n_buffer);
    usr_buf = (BUFTYPE *)calloc(reqbufs.count, sizeof(BUFTYPE));
    if (usr_buf == NULL)
    {
        debug_print("Out of memory\n");
        exit(-1);
    }
    for (n_buffer = 0; n_buffer < reqbufs.count; ++n_buffer)
    {
        struct v4l2_buffer buf;
        memset(&buf, 0, sizeof(buf));
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = n_buffer;
        if (-1 == ioctl(fd, VIDIOC_QUERYBUF, &buf))
        {
            debug_print("Fail to ioctl : VIDIOC_QUERYBUF");
            exit(EXIT_FAILURE);
        }
        usr_buf[n_buffer].length = buf.length;
        usr_buf[n_buffer].start = (char *)mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, buf.m.offset);
        if (MAP_FAILED == usr_buf[n_buffer].start)
        {
            debug_print("Fail to mmap");
            exit(EXIT_FAILURE);
        }
    }
}

int open_camera(void)
{
    int fd;
    struct v4l2_input inp;
    //| O_NONBLOCK
    fd = open(FILE_VIDEO, O_RDWR, 0);
    if (fd < 0)
    {
        debug_print("%s open err \n", FILE_VIDEO);
        exit(EXIT_FAILURE);
    }
    inp.index = 0;
    if (-1 == ioctl(fd, VIDIOC_S_INPUT, &inp))
    {
        debug_print("VIDIOC_S_INPUT \n");
    }
    return fd;
}

void init_camera(int fd)
{
    struct v4l2_capability cap;
    struct v4l2_format tv_fmt;
    struct v4l2_fmtdesc fmtdesc;
    struct v4l2_control ctrl;
    int ret;
    memset(&fmtdesc, 0, sizeof(fmtdesc));
    fmtdesc.index = 0;
    fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    /*  Querying the capabilities of cameral to check if it supports V4L2_BUF_TYPE_VIDEO_CAPTURE and V4L2_CAP_STREAMING 
        wchich could allow us to get the stream from the cameral.*/
    if (ret = ioctl(fd, VIDIOC_QUERYCAP, &cap) < 0)
    {
        debug_print("fail to ioctl VIDEO_QUERYCAP \n");
        exit(EXIT_FAILURE);
    }
    if (!(cap.capabilities & V4L2_BUF_TYPE_VIDEO_CAPTURE))
    {
        debug_print("The Current device is not a video capture device \n");
        exit(EXIT_FAILURE);
    }
    if (!(cap.capabilities & V4L2_CAP_STREAMING))
    {
        debug_print("The Current device does not support streaming i/o\n");
        exit(EXIT_FAILURE);
    }
    /*  print the info of the camera.*/
    debug_print("\ncamera driver name is : %s", cap.driver);
    debug_print("camera device name is : %s", cap.card);
    debug_print("camera bus information: %s", cap.bus_info);
    /* print the devices and format which are supported by cameral.*/
    while (ioctl(fd, VIDIOC_ENUM_FMT, &fmtdesc) != -1)
    {
        debug_print("support device %d.%s", fmtdesc.index + 1, fmtdesc.description);
        fmtdesc.index++;
    }
    /* set the buffer type , the pixel of one frame and pixelformat including YUVY YUV etc.*/
    tv_fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    tv_fmt.fmt.pix.width = FRAME_PIX_WIDTH;
    tv_fmt.fmt.pix.height = FRAME_PIX_HEIGHT;
    tv_fmt.fmt.pix.pixelformat = PIXELFORMAT;
    /*  no clear for field*/
    tv_fmt.fmt.pix.field = V4L2_FIELD_NONE;
    if (ioctl(fd, VIDIOC_S_FMT, &tv_fmt) < 0)
    {
        debug_print("VIDIOC_S_FMT set err");
        exit(-1);
        close(fd);
    }
    init_mmap(fd);
}

void start_capture(int fd)
{
    unsigned int i;
    enum v4l2_buf_type type;
    for (i = 0; i < n_buffer; i++)
    {
        struct v4l2_buffer buf;
        memset(&buf, 0, sizeof(buf));
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;
        if (-1 == ioctl(fd, VIDIOC_QBUF, &buf))
        {
            perror("Fail to ioctl 'VIDIOC_QBUF'");
            exit(EXIT_FAILURE);
        }
    }
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (-1 == ioctl(fd, VIDIOC_STREAMON, &type))
    {
        debug_print("i=%d.\n", i);
        debug_print("VIDIOC_STREAMON");
        close(fd);
        exit(EXIT_FAILURE);
    }
}

void read_frame(int fd, uint8_t *img)
{
    struct v4l2_buffer buf;
    unsigned int i;
    memset(&buf, 0, sizeof(buf));
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    if (-1 == ioctl(fd, VIDIOC_DQBUF, &buf))
    {
        perror("Fail to ioctl 'VIDIOC_DQBUF'");
        exit(EXIT_FAILURE);
    }
    assert(buf.index < n_buffer);
    debug_print("get buffer from the kernel memory and size = %d.", usr_buf[buf.index].length);
    memcpy(img, usr_buf[buf.index].start, usr_buf[buf.index].length);
    //img = (uint8_t *)usr_buf[buf.index].start;  segement fault because usr_buff memory may be freed.
    if (-1 == ioctl(fd, VIDIOC_QBUF, &buf))
    {
        debug_print("Fail to ioctl 'VIDIOC_QBUF'");
        exit(EXIT_FAILURE);
    }
}

void close_camera_device(int fd)
{
    unsigned int i;
    stop_capture(fd);
    for (i = 0; i < n_buffer; i++)
    {
        if (-1 == munmap(usr_buf[i].start, usr_buf[i].length))
        {
            exit(-1);
        }
    }
    free(usr_buf);
    if (-1 == close(fd))
    {
        debug_print("Fail to close fd");
        exit(EXIT_FAILURE);
    }
}
