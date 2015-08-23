cmake_minimum_required(VERSION 3.3)
project(aria)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(Boost 1.59.0)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    set(SOURCE_FILES src/base/message.cpp src/base/message.h)
    add_library(aria STATIC ${SOURCE_FILES} src/base/message.cpp src/base/message.h)
else()
    message(FATAL_ERROR "Failed to find Boost libraries. Unable to build.")
endif()
