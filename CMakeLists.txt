cmake_minimum_required(VERSION 2.8.4)
project(kalculator)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

add_custom_target(kalculator COMMAND make -C ${kalculator_SOURCE_DIR}
        CLION_EXE_DIR=${PROJECT_BINARY_DIR})