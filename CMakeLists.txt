cmake_minimum_required(VERSION 3.3)
project(aria)

set(SOURCE_FILES
    src/base/message.cpp
    src/base/message.hpp
    src/base/types.hpp
    src/util/binary_buffer.hpp
    src/util/binary_buffer.cpp
)

set(UNIT_TEST_FILES
    tests/unit/unit.cpp
    tests/unit/util/binary_buffer_test.cpp
    tests/unit/base/message_test.cpp
)

find_package(Boost 1.59.0)
if(Boost_FOUND)
    include_directories(src ${Boost_INCLUDE_DIRS})
    add_library(aria STATIC ${SOURCE_FILES})
    set_property(TARGET aria PROPERTY CXX_STANDARD 14)

    # Tests
    add_subdirectory(tests/gtest)
    include_directories(${gtest_SOURCE_DIR}/include)

    # Unit tests
    add_executable(unit_tests ${UNIT_TEST_FILES})
    target_link_libraries(unit_tests aria gtest gtest_main)
    set_property(TARGET unit_tests PROPERTY CXX_STANDARD 14)
    add_custom_target(unit COMMAND unit_tests)

    # Run all tests using the 'check' target
    add_custom_target(check COMMAND unit_tests)
else()
    message(FATAL_ERROR "Failed to find Boost libraries. Unable to build.")
endif()
