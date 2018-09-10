# CAMERAL-NETWORK
THREE STEPS TO CREATE A CAMERAL NETWORK.

> 当前只初步完成了摄像头以30fps捕获图像，输出yuyv格式。然后进行格式转换，yuyv转成264.最后用rtp传输到目标主机。

## 项目简介（开发平台ubuntu 18）

### 1. 使用的库有v4l2、x264、jrtplib

#### 1.1 v4l2

* 1.1.1 简介
* 1.1.2 参考

#### 1.2 x264

* 1.2.1 编译
* 1.2.2 安装

#### 1.3 jrtplib

* 1.3.1 编译
* 1.3.2 安装

### 2. 目录介绍

* include:头文件目录
    * config.h 配置文件
    * debug.h debug打印输出

* res: 包含一些资源

* test:测试目录，包含整个流程的测试。通过test_min_level和test_max_level来配置测试的流程。具体可以看./test/test_main.cpp代码。
    * 摄像头捕获测试
    * yuyv转成yuv420p测试
    * yuv420p转成264测试
    * 264传输测试

* *.cpp文件:源码

### 3. 编译和运行

* 如果想要分段测试的话，将main.cpp中main函数的run()注释掉。反之，运行的话，则取消注释。
* make
* 运行（下面三个步骤不分先后）
    * 连上本地的摄像头
    * ./main
    * 打开用户终端的接收264流的软件，比如vlc。

## TODO:三个步骤即可创建一个摄像头到用户终端的显示，适合的场景比如：车载摄像头

---

### 1. 摄像头配置

* 函数名:camera_struct camera(camera_struct *cam);

* 设置参数，比如频率、捕获的图片格式等等。

### 2. 格式转换

* 函数名:format_struct convert_format(format_struct *for);

* 设置转换目标格式，比如264、jpeg、m3u8等等。

### 3. 传输

* 函数名:tran_struct transmit(tran_strcut *tran);

* 选择传输协议，比如rtp、rtmp、flv等等。




