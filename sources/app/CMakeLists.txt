include_directories(${LIB_INCLUDE_DIR})
add_executable(rh_diff
    src/main.cpp)
target_link_libraries(rh_diff rh_lib)
