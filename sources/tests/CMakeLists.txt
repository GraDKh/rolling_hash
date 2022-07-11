include_directories(${LIB_INCLUDE_DIR})
add_executable(rh_tests
    src/delta_builder_tests.cpp
    src/file_reader_tests.cpp
    src/file_writer_tests.cpp
    src/hasher_tests.cpp
    # Note: we can't use main file from gtest since at the point of generation 
    # gtest sources aren't downloaded yet.
    src/main.cpp
    src/mock_reader.cpp
    src/signature_creator_tests.cpp
    src/signature_tests.cpp)
add_dependencies(rh_tests googletest)
target_link_libraries(rh_tests rh_lib gtest pthread)

add_custom_target(run_tests COMMAND "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/rh_tests")
add_dependencies(run_tests rh_tests)
