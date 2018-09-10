#ifndef TEST_MAIN_H
#define TEST_MAIN_H

void capture_frames(int num_frames, char *file_name);
void yuyv_yuv_test(char *FILE_SRC,char *FILE_DST);
void yuv_x264_test(char *FILE_SRC,char *FILE_DST);
void test_main(int test_min_level,int test_max_level,int capture_frames_num,char *capture_save_file,char *yuv_save_file,char *save_264_file);
#endif