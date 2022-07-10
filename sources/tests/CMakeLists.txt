include_directories(${LIB_INCLUDE_DIR})
add_executable(rh_tests
    delta_builder_tests.cpp
    file_reader_tests.cpp
    file_writer_tests.cpp
    hasher_tests.cpp
    main.cpp
    mock_reader.cpp
    signature_creator_tests.cpp
    signature_tests.cpp)
add_dependencies(rh_tests googletest)
target_link_libraries(rh_tests rh_lib gtest pthread)

add_custom_target(run_tests COMMAND "${CMAKE_CURRENT_BINARY_DIR}/rh_tests")
add_dependencies(run_tests rh_tests)
