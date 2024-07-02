//
// Created by gzyit on 2024/7/2.
//

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <opencv2/opencv.hpp>
#include <utility>
#include <iostream>
#include <chrono>
#include <cstdio>
#include <thread> // 添加线程库
#include <mutex> // 添加互斥锁库

namespace py = pybind11;


// 将fourcc转换为字符串
std::string fourccToString(int fourcc) {
    char arr[5];
    arr[3] = (fourcc >> 24) & 0xFF;
    arr[2] = (fourcc >> 16) & 0xFF;
    arr[1] = (fourcc >> 8) & 0xFF;
    arr[0] = fourcc & 0xFF;
    arr[4] = '\0'; // Null terminator for C string

    printf("%d %d %d %d\n", arr[0], arr[1], arr[2], arr[3]);
    return std::string(arr);
}

std::mutex mtx; // 用于保护frame的互斥锁
cv::Mat frame; // 全局变量，用于跨线程共享帧数据
// 新线程处理函数
void captureFrames(cv::VideoCapture &cap) {
    while (true) {
        auto start = std::chrono::high_resolution_clock::now();
        if (!cap.read(frame)) {
            std::cerr << "read frame failed!" << std::endl;
            break;
        }
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        std::cout << "Function execution time: " << duration.count() << " ms" << std::endl;
    }
    cap.release(); // 确保在子线程中释放资源
}


int start() {
    // 释放GIL，避免Python代码阻塞
    py::gil_scoped_release release;


    // 初始化视频捕获对象，0代表默认摄像头，如果有多个摄像头，可以尝试1, 2, ...
    cv::VideoCapture cap(0, cv::CAP_DSHOW);

    if (!cap.isOpened()) {
        std::cerr << "can't open camera！" << std::endl;
        return -1;
    }

    // 设置视频的帧率、宽度和高度
//    int fps = 30;
//    int width = 1920;
//    int height = 1080;
    int fps = 60;
    int width = 640;
    int height = 480;
    cap.set(cv::CAP_PROP_FPS, fps);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, width);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, height);
    // 设置视频的编码格式
    cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));

    // 获取视频的帧率、宽度和高度、编码格式
    std::cout << "fps:" << cap.get(cv::CAP_PROP_FPS) << std::endl;
    std::cout << "width:" << cap.get(cv::CAP_PROP_FRAME_WIDTH) << std::endl;
    std::cout << "height:" << cap.get(cv::CAP_PROP_FRAME_HEIGHT) << std::endl;
    std::cout << "fourcc:" << fourccToString(cap.get(cv::CAP_PROP_FOURCC)) << std::endl;

    // 创建一个窗口用来显示视频
    cv::namedWindow("capture", cv::WINDOW_NORMAL);
    cv::resizeWindow("capture", static_cast<int>(width), static_cast<int>(height));


    // 启动子线程进行帧捕捉
    std::thread captureThread(captureFrames, std::ref(cap));

    while (true) {
        auto start = std::chrono::high_resolution_clock::now();
        // 保护访问共享的frame
        mtx.lock();
        if (!frame.empty()) {
            cv::imshow("capture", frame);
        }
        mtx.unlock();

        int key = cv::waitKey(1) & 0xFF;
        if (key == 'q' || key == 27) {
            break;
        }
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
//        std::cout << "Function execution time: " << duration.count() << " ms" << std::endl;
    }

    // 等待子线程结束
    captureThread.join();


    // 释放窗口
    cv::destroyAllWindows();
    return 0;
}

PYBIND11_MODULE(camera2, m) {
    m.doc() = "A module for displaying images with OpenCV";
    m.def("start", &start, "Display an image using OpenCV");
}
