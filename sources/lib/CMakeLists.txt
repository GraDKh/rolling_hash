set(LIB_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(LIB_INCLUDE_DIR ${LIB_INCLUDE_DIR} PARENT_SCOPE)
include_directories(${LIB_INCLUDE_DIR})
add_library(rh_lib
    src/file_reader.cpp
    src/file_writer.cpp
    src/signature.cpp
)
target_link_libraries(rh_lib hash_lib)
