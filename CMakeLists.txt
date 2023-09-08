cmake_minimum_required(VERSION 3.22)
set(This signal_processing)
project(${This} C CXX)
find_package(GTest CONFIG REQUIRED)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

include_directories(gtest/include)
include_directories(.)

# Main executable.
add_library(
        ${This}
        buffer.h
        filter.h
        least_mean_squares.h
)

set_target_properties(${This} PROPERTIES LINKER_LANGUAGE C)

# Add tests.
set(UnitTests ${This}_unit_tests)
add_executable(
        ${UnitTests}
        buffer_test.cpp
        filter_test.cpp
        least_mean_squares_test.cpp
)

set_target_properties(${UnitTests} PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(${UnitTests}
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(${UnitTests})