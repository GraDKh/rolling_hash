include(ExternalProject)

set(THIRDPARTY_LIB_DIR "${CMAKE_BINARY_DIR}/3rdparty")
file(MAKE_DIRECTORY ${THIRDPARTY_LIB_DIR})

set(GTEST_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/gtest")
ExternalProject_Add(googletest
    SOURCE_DIR        "${GTEST_ROOT}"
    GIT_REPOSITORY    "https://github.com/google/googletest.git"
    GIT_TAG           "release-1.12.1 "
    INSTALL_COMMAND   bash -c "cp ./lib/* ${THIRDPARTY_LIB_DIR}")
set(GTEST_SRC "${GTEST_ROOT}/googletest/src" PARENT_SCOPE)
set(THIRDPARTY_INCLUDE_DIRS  ${THIRDPARTY_INCLUDE_DIRS} "${GTEST_ROOT}/googletest/include")

set(THIRDPARTY_LIB_DIR "${THIRDPARTY_LIB_DIR}" PARENT_SCOPE)
set(THIRDPARTY_INCLUDE_DIRS "${THIRDPARTY_INCLUDE_DIRS}" PARENT_SCOPE)