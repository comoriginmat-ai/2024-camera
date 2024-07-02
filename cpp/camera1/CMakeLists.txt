cmake_minimum_required(VERSION 3.28)
project(camera1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(pybind11 REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Freetype REQUIRED)


pybind11_add_module(camera1 my_cpp_module.cpp cvxFont.cpp)
target_link_libraries(camera1 PRIVATE ${OpenCV_LIBS} Freetype::Freetype)


