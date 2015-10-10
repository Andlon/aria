cmake_minimum_required(VERSION 3.3)
project(aria)

set(LIB_SOURCE_FILES
    include/aria/types.hpp
    src/util/binary_buffer.hpp
    src/util/binary_buffer.cpp
    src/util/unique_ptr_tools.hpp
)

set(UNIT_TEST_FILES
    tests/unit/unit.cpp
    tests/unit/util/binary_buffer_test.cpp
)

find_package(Boost 1.59.0 REQUIRED COMPONENTS system)
if(Boost_FOUND)
    include_directories(include src ${Boost_INCLUDE_DIRS})
    add_library(aria STATIC ${LIB_SOURCE_FILES})
    add_executable(aria_speaker src/speaker_main.cpp)
    add_executable(aria_source src/source_main.cpp)
    target_link_libraries(aria_speaker aria ${Boost_LIBRARIES})
    target_link_libraries(aria_source aria ${Boost_LIBRARIES})
    set_property(TARGET aria PROPERTY CXX_STANDARD 11)
    set_property(TARGET aria_speaker PROPERTY CXX_STANDARD 11)
    set_property(TARGET aria_source PROPERTY CXX_STANDARD 11)

    # Tests
    add_subdirectory(tests/gtest)
    include_directories(${gtest_SOURCE_DIR}/include)

    # Unit tests
    add_executable(unit_tests ${UNIT_TEST_FILES})
    target_link_libraries(unit_tests aria gtest gtest_main)
    set_property(TARGET unit_tests PROPERTY CXX_STANDARD 11)
    add_custom_target(unit COMMAND unit_tests)

    # Run all tests using the 'check' target
    add_custom_target(check COMMAND unit_tests)
else()
    message(FATAL_ERROR "Failed to find Boost libraries. Unable to build.")
endif()

