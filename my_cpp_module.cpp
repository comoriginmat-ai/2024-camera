#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <opencv2/opencv.hpp>
#include <utility>
#include "cvxFont.h"
#include <iostream>
#include <chrono>
#include <cstdio>

namespace py = pybind11;

// 使用单例模式管理字体实例
class FontManager {
public:
    static cvx::CvxFont& GetInstance() {
        static cvx::CvxFont instance("Noto.ttf"); // 静态局部变量，只会初始化一次
        return instance;
    }
};


cv::String msg = "这段代码首先初始化FreeType库，然后加载宋体字体文件，并设置字体大小。";

cv::Mat numpy_to_mat(py::array_t<unsigned char> img) {

    const Py_ssize_t * shape = img.shape();
    int height = shape[0];
    int width = shape[1];
    int channels = img.ndim() == 3 ? shape[2] : 1;


//    auto strides = img.strides();
//    auto dtype = img.dtype();
//    size_t itemsize = dtype.itemsize();
//
//
//    auto row_bytes = width * channels * itemsize;
//
//
//    if (strides[0] != row_bytes) {
//        throw std::runtime_error("Non-contiguous array passed!");
//    }



    uchar *data = reinterpret_cast<uchar *>(img.mutable_data());
    size_t step = static_cast<size_t>(img.strides(0)); // 获取行间距


     cv::Mat mat(height, width, CV_8UC(channels), data, step);



//    if (channels == 3) {
//        cv::cvtColor(mat, mat, cv::COLOR_RGB2BGR);
//    }

    return mat;
}

void display_image(py::array_t<unsigned char> img) {
    cv::Mat mat = numpy_to_mat(std::move(img));

//    auto start = std::chrono::high_resolution_clock::now();
    cvx::CvxFont& font = FontManager::GetInstance();


//    auto stop = std::chrono::high_resolution_clock::now();
//    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
//    std::cout << "Function execution time: " << duration.count() << " ms" << std::endl;


    putText(mat, msg, cv::Point(100, 100), font, 30, cv::Scalar(255, 255, 255));
    putText(mat, msg, cv::Point(100, 200), font, 30, cv::Scalar(255, 255, 255));
    putText(mat, msg, cv::Point(100, 300), font, 30, cv::Scalar(255, 255, 255));
    putText(mat, msg, cv::Point(100, 400), font, 30, cv::Scalar(255, 255, 255));

    putText(mat, msg, cv::Point(100, 500), font, 30, cv::Scalar(255, 255, 255));
    putText(mat, msg, cv::Point(100, 600), font, 30, cv::Scalar(255, 255, 255));
    putText(mat, msg, cv::Point(100, 700), font, 30, cv::Scalar(255, 255, 255));
    putText(mat, msg, cv::Point(100, 800), font, 30, cv::Scalar(255, 255, 255));
    cv::imshow("Displayed Image", mat);
    cv::waitKey(1);
}

PYBIND11_MODULE(camera, m) {
    m.doc() = "A module for displaying images with OpenCV";
    m.def("display_image", &display_image, "Display an image using OpenCV",py::arg("frame"));
}
