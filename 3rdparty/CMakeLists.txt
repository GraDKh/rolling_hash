include(ExternalProject)

set(THIRDPARTY_LIB_DIR "${CMAKE_BINARY_DIR}/3rdparty/lib")
file(MAKE_DIRECTORY ${THIRDPARTY_LIB_DIR})

set(THIRDPARTY_SRC_DIR "${CMAKE_BINARY_DIR}/3rdparty/src")
file(MAKE_DIRECTORY ${THIRDPARTY_SRC_DIR})

set(GTEST_ROOT "${THIRDPARTY_SRC_DIR}/gtest")
ExternalProject_Add(googletest
    SOURCE_DIR        "${GTEST_ROOT}"
    GIT_REPOSITORY    "https://github.com/google/googletest.git"
    GIT_TAG           "release-1.12.1"
    GIT_SHALLOW       TRUE
    INSTALL_COMMAND   bash -c "cp ./lib/* \"${THIRDPARTY_LIB_DIR}\"")
set(THIRDPARTY_INCLUDE_DIRS  ${THIRDPARTY_INCLUDE_DIRS} "${GTEST_ROOT}/googletest/include")

set(HASH_LIBRARY_ROOT "${THIRDPARTY_SRC_DIR}/hash-library")
ExternalProject_Add(hash_library
    SOURCE_DIR        "${HASH_LIBRARY_ROOT}"
    GIT_REPOSITORY    "https://github.com/stbrumme/hash-library.git"
    GIT_TAG           "hash_library_v8"
    GIT_SHALLOW       TRUE
    PATCH_COMMAND     bash -c "cp \"${CMAKE_CURRENT_SOURCE_DIR}/hash-library/CMakeLists.txt\" \"${HASH_LIBRARY_ROOT}/CMakeLists.txt\""
    INSTALL_COMMAND   bash -c "cp ./lib/* ${THIRDPARTY_LIB_DIR}"
    )
set(THIRDPARTY_INCLUDE_DIRS  ${THIRDPARTY_INCLUDE_DIRS} "${HASH_LIBRARY_ROOT}/")

set(THIRDPARTY_LIB_DIR "${THIRDPARTY_LIB_DIR}" PARENT_SCOPE)
set(THIRDPARTY_INCLUDE_DIRS "${THIRDPARTY_INCLUDE_DIRS}" PARENT_SCOPE)