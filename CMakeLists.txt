cmake_minimum_required(VERSION 3.28)
project(camera)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(pybind11 REQUIRED)
find_package(OpenCV REQUIRED)


pybind11_add_module(camera my_cpp_module.cpp)
target_link_libraries(camera PRIVATE ${OpenCV_LIBS})


