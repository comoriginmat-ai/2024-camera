#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <opencv2/opencv.hpp>
#include <utility>

namespace py = pybind11;

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
    cv::imshow("Displayed Image", mat);
    cv::waitKey(1);
}

PYBIND11_MODULE(camera, m) {
    m.doc() = "A module for displaying images with OpenCV";
    m.def("display_image", &display_image, "Display an image using OpenCV",py::arg("frame"));
}
