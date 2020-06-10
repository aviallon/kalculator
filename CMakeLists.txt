cmake_minimum_required(VERSION 2.8.4)
project(kalculator)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -w")

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(DEBUG_DISABLED "-DDISABLE_DEBUG")
else()
    set(DEBUG_DISABLED "")
endif()

add_custom_target(kalculator COMMAND make DEBUG=${DEBUG_DISABLED} -B -C ${kalculator_SOURCE_DIR}
        CLION_EXE_DIR=${PROJECT_BINARY_DIR})

add_custom_target(package COMMAND make DEBUG=${DEBUG_DISABLED} -B -C ${kalculator_SOURCE_DIR}
        CLION_EXE_DIR=${PROJECT_BINARY_DIR} package)