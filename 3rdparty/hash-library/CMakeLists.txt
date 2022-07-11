cmake_minimum_required(VERSION 3.8)

project(hash_library)

# As for now we need SHA1 hash only.
# Hash library supports building any susbet of hash algorithms.
# If any other algorithms are needed they should be added here.
add_library(hash_lib
    sha1.cpp
)

# Write artefacts to a separate folder to simplify copying to 3rdparty lib dir.
set(LIB_DIR "${CMAKE_CURRENT_BINARY_DIR}/lib")
set_target_properties(hash_lib PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${LIB_DIR}"
    ARCHIVE_OUTPUT_DIRECTORY "${LIB_DIR}")