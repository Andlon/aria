cmake_minimum_required(VERSION 3.3)
project(aria)

set(SOURCE_FILES
    src/base/message.cpp
    src/base/message.hpp
    src/base/types.hpp
    src/util/binary_buffer.hpp
    src/util/binary_buffer.cpp
)

find_package(Boost 1.59.0)
if(Boost_FOUND)
    include_directories(src ${Boost_INCLUDE_DIRS})
    add_library(aria STATIC ${SOURCE_FILES})
    set_property(TARGET aria PROPERTY CXX_STANDARD 14)
else()
    message(FATAL_ERROR "Failed to find Boost libraries. Unable to build.")
endif()
