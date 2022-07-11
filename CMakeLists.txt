cmake_minimum_required(VERSION 3.8)

project(rolling_hash)

add_compile_options(-std=c++17 -Wall -Werror)

add_subdirectory(3rdparty)
include_directories(SYSTEM ${THIRDPARTY_INCLUDE_DIRS})
link_directories(${THIRDPARTY_LIB_DIR})

add_subdirectory(sources)