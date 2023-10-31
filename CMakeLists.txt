cmake_minimum_required(VERSION 3.0)
project( detectFaces )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( detectFaces detectFaces.cpp )
target_link_libraries( detectFaces ${OpenCV_LIBS} )
set(CMAKE_BUILD_TYPE Debug)

