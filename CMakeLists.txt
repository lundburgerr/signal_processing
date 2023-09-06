cmake_minimum_required(VERSION 3.22)
project(signal_processing C CXX)
find_package(GTest CONFIG REQUIRED)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

include_directories(gtest/include)
include_directories(.)

add_executable(
        signal_processing
        buffer.h
)

set_target_properties(signal_processing PROPERTIES LINKER_LANGUAGE C)

add_executable(
        unit_test
        buffer_test.cpp
)

set_target_properties(unit_test PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(unit_test
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(unit_test)