cmake_minimum_required(VERSION 2.6.4)

project(Utils)

option(USE_VECTORIZATION_SIMD "AVX2 on X64 and NEON on ARM" ON)

if(USE_VECTORIZATION_SIMD)
    add_definitions(-DAVX2)
    add_definitions(-mavx2)
endif(USE_VECTORIZATION_SIMD)

set(LIBRARY_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})

include_directories(include)

file(GLOB_RECURSE
    source_files
    src/*
    include/*)
     
add_library(Utils
    STATIC
    ${source_files})

set_property(TARGET Utils PROPERTY CXX_STANDARD 14)
    
assign_source_group(${source_files})

add_subdirectory(test)