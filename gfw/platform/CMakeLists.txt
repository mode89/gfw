project(GFW_PLATFORM)
cmake_minimum_required(VERSION 2.8)

if(WIN32)
    file(GLOB PLATFORM_SOURCE_FILES win/*.cpp)
endif(WIN32)

add_library(gfw_platform ${PLATFORM_SOURCE_FILES})
