cmake_minimum_required(VERSION 3.8)

project(hash_library)

add_library(hash_lib
    sha1.cpp
)

# Write artefacts to a separate folder to simplify copying to 3rdparty lib dir
set(LIB_DIR "${CMAKE_CURRENT_BINARY_DIR}/lib")
set_target_properties(hash_lib PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${LIB_DIR}"
    ARCHIVE_OUTPUT_DIRECTORY "${LIB_DIR}")