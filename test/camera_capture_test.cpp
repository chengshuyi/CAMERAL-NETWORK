#include "stdint.h"
#include "time.h"

#include "config.h"
#include "test_main.h"
#include "camera_capture.h"
#include "debug.h"

void capture_frames(int num_frames, char *file_name)
{
    int fd;
    int r;
    uint8_t *yuyv_img;
    fd_set fds;
    struct timeval tv;
    FILE *f_save;
    yuyv_img = (uint8_t *)malloc(sizeof(uint8_t) * YUYV_SIZE);
    f_save = fopen(file_name,"wb");
    fd = open_camera();
    init_camera(fd);
    start_capture(fd);
    while (num_frames--)
    {
        FD_ZERO(&fds);
        FD_SET(fd, &fds);
        tv.tv_sec = 2;
        tv.tv_usec = 0;
        r = select(fd + 1, &fds, NULL, NULL, &tv);
        if (-1 == r)
        {
            debug_print("Fail to select");
            exit(EXIT_FAILURE);
        }
        if (0 == r)
        {
            debug_print("select Timeout");
            exit(-1);
        }
        read_frame(fd, yuyv_img);
        fwrite(yuyv_img,YUYV_SIZE,1,f_save);
        debug_print("capture one frame.");
    }
    close_camera_device(fd);
    fcloseall();
}
