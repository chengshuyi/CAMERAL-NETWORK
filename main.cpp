/**
 * @brief 
 * 
 * @file main.cpp
 * @author your name
 * @date 2018-09-07
 */

#include "state_machine.h"
#include "test_main.h"
#include "stdio.h"

void run()
{
    to_state(INIT_CAMERA);

    for (;;)
    {
        do_state();
    }
}

int main(int argc, char **argv)
{
    int test_min_level,test_max_level;
    char capture_save_file[] = "./res/image.yuyv";
    char yuv_save_file[] = "./res/image.yuv";
    char save_264_file[] = "./res/image.264";
    run();
    if (argc > 1)
    {
        sscanf(argv[1], "%d,%d", &test_min_level,&test_max_level);
    }
    else
    {
        test_min_level = 0;
    }
    test_main(test_min_level,test_max_level,100,capture_save_file,yuv_save_file,save_264_file);
    return 0;
}
/**
 * ffplay -f rawvideo -pixel_format yuyv422 -video_size 640x480 image.yuyv
 * ffplay -f rawvideo -pixel_format yuv420p -video_size 640x480 image.yuv
 * ffplay -stats -f h264 image.264
 * 
 */