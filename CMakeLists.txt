cmake_minimum_required(VERSION 3.9)
project(test_cudastd_msvc LANGUAGES CXX CUDA)

set(CMAKE_CUDA_SEPARABLE_COMPILATION ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CUDA_ARCHITECTURES 86)

add_executable(test_cudastd_msvc
    test.cu
    test.cpp
    test.hpp
)
target_include_directories(test_cudastd_msvc PRIVATE ${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES})
