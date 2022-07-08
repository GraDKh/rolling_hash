add_executable(tests
    buffered_file_reader_tests.cpp
    main.cpp)
add_dependencies(tests googletest)
target_link_libraries(tests gtest pthread)
