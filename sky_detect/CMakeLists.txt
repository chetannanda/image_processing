cmake_minimum_required (VERSION 2.6)
project(sky-detect)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)
find_package(OpenCV REQUIRED)

set(CMAKE_CXX_FLAGS "-g -Wall")
add_subdirectory(src)
