#include "stdint.h"

#include "state_machine.h"
#include "camera_capture.h"
#include "yuyv2yuv.h"
#include "yuv2x264.h"
#include "transfer.h"
#include "debug.h"
#include "config.h"

static int STATIC_STATE = INIT_CAMERA;
int fd;
uint8_t *yuyv_img, *yuv_img;

void to_state(int state)
{
    STATIC_STATE = state;
}

void do_state()
{
    int ret;
    int i;
    struct timeval tv;
    int r;
    uint8_t dst_ip[] = DEST_IP;
    switch (STATIC_STATE)
    {
    case INIT_CAMERA:
    {
        debug_print("INIT_CAMERA state, next to READ_CAMERA state.\n");
        /* init camera.*/
        yuyv_img = (uint8_t *)malloc(sizeof(uint8_t) * YUYV_SIZE);
        yuv_img = (uint8_t *)malloc(sizeof(uint8_t) * YUV_SIZE);
        fd = open_camera();
        init_camera(fd);
        start_capture(fd);
        /* init x264*/
        ret = init_param_for_x264();
        if (ret < 0)
        {
            debug_print("init param for x264 has caused fault.");
            to_state(ERROR_STATE);
        }
        /* init jrtp*/
        ret = init_param_for_jrtplib(dst_ip, LOCAL_PORT, REMOTE_PORT, FPS);
        if (ret < 0)
        {
            debug_print("init param for jrtp has caused fault.");
            to_state(ERROR_STATE);
        }
        to_state(READ_CAMERA);
        break;
    }
    case READ_CAMERA:
    {
        debug_print("READ_CAMERA state, next to TRAN_FORMAT state.");
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(fd, &fds);
        tv.tv_sec = 2;
        tv.tv_usec = 0;
        r = select(fd + 1, &fds, NULL, NULL, &tv);
        if (-1 == r)
        {
            if (EINTR == errno)
            {
                to_state(READ_CAMERA);
            }
            debug_print("Fail to select");
            exit(EXIT_FAILURE);
        }
        if (0 == r)
        {
            debug_print("select Timeout");
            exit(-1);
        }
        read_frame(fd, yuyv_img);
        to_state(TRAN_FORMAT);
        break;
    }
    case TRAN_FORMAT:
    {
        debug_print("TRAN_FORMAT state, next to SEND_PACKET state.");
        /**/
        yuyv_2_yuv(yuyv_img, yuv_img);
        /**/
        ret = encode_one_frame(yuv_img);
        if (ret < 0)
        {
            debug_print("encode one frame has caused fault.");
            to_state(ERROR_STATE);
        }
        to_state(SEND_PACKET);
        break;
    }
    case SEND_PACKET:
    {
        debug_print("SEND_PACKET state, next to READ_CAMERA state.");
        for (i = 0; i < i_nal; ++i)
        {
            debug_print("transfer %dth nalu  size = %d", i, nal[i].i_payload);
            ret = transfer_one_nalu_by_rtp(nal[i].p_payload, nal[i].i_payload, i==0?(900/FPS):0);
            if (ret < 0)
            {
                to_state(ERROR_STATE);
            }
        }
        to_state(READ_CAMERA);
        break;
    }
    case ERROR_STATE:
    {
        close_camera_device(fd);
        close_yuv2x264();
        close_transfer();
        //to_state(INIT_CAMERA);
        break;
    }
    default:
    {
        debug_print("default state , next to ERROR_STATE state.");
        to_state(ERROR_STATE);
        break;
    }
    }
}