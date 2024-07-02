cmake_minimum_required(VERSION 3.28)
project(camera2)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(pybind11 REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Freetype REQUIRED)


pybind11_add_module(camera2 camera.cpp)
target_link_libraries(camera2 PRIVATE ${OpenCV_LIBS} Freetype::Freetype)
