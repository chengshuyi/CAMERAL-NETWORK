#ifndef CONFIG_H
#define CONFIG_H

#define FRAME_PIX_WIDTH     640
#define FRAME_PIX_HEIGHT    480
#define PIXELFORMAT         V4L2_PIX_FMT_YUYV
#define FILE_VIDEO          "/dev/video1"
/**
 * @brief 
 * 
 */

#define YUYV_SIZE           (FRAME_PIX_WIDTH*FRAME_PIX_HEIGHT*2)
#define YUV_SIZE            (FRAME_PIX_WIDTH*FRAME_PIX_HEIGHT*3/2)
/**
 * @brief 
 * 
 */

//#define DEST_IP             {127,0,0,1}
#define DEST_IP             {192,168,137,2}
#define LOCAL_PORT          3000
#define REMOTE_PORT         4000
#define FPS                 30

#endif
