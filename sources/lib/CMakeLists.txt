set(LIB_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(LIB_INCLUDE_DIR ${LIB_INCLUDE_DIR} PARENT_SCOPE)
include_directories(${LIB_INCLUDE_DIR})
add_library(rh_lib
    src/file_delta_builder.cpp
    src/file_operations.cpp
    src/file_reader.cpp
    src/file_writer.cpp
    src/memory_delta_builder.cpp
    src/signature.cpp
)
add_dependencies(rh_lib hash_library)
target_link_libraries(rh_lib hash_lib)
