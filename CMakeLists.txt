cmake_minimum_required(VERSION 3.22)
project(signal_processing C CXX)
find_package(GTest CONFIG REQUIRED)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

include_directories(gtest/include)
include_directories(.)

# Main executable.
add_executable(
        signal_processing
        buffer.h
        filter.h
)

set_target_properties(signal_processing PROPERTIES LINKER_LANGUAGE C)

# Add tests.
add_executable(
        unit_tests
        buffer_test.cpp
        filter_test.cpp
)

set_target_properties(unit_tests PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(unit_tests
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(unit_tests)