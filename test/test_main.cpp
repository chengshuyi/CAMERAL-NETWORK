#include "stdio.h"
#include "test_main.h"

void test_main(int test_min_level, int test_max_level, int capture_frames_num, char *capture_save_file, char *yuv_save_file, char *save_264_file)
{
    if (test_min_level <= 0 && test_max_level >= 0)
    {
        capture_frames(capture_frames_num, capture_save_file);
    }

    if (test_min_level <= 1 && test_max_level >= 1)
    {
        yuyv_yuv_test(capture_save_file, yuv_save_file);
    }

    if (test_min_level <= 2 && test_max_level >= 2)
    {
        yuv_x264_test(yuv_save_file, save_264_file);
    }
}