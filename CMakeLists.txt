cmake_minimum_required(VERSION 3.20)

set(CMAKE_C_COMPILER gcc-11 CACHE STRING "Pinned C compiler")
set(CMAKE_CXX_COMPILER g++-11 CACHE STRING "Pinned C++ compiler")

project(
    MBP_T02_ChildLock
    VERSION 1.0
    DESCRIPTION "UC01 Electronic Child Lock System"
    LANGUAGES C CXX)

include(FetchContent)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(CHILD_LOCK_ENABLE_COVERAGE "Enable gcov coverage flags" OFF)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
    GIT_SHALLOW TRUE)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_library(
    child_lock_system
    src/child_lock_system.c)

target_include_directories(
    child_lock_system
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include)

target_compile_options(
    child_lock_system
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic)

if(CHILD_LOCK_ENABLE_COVERAGE)
    target_compile_options(
        child_lock_system
        PRIVATE
            --coverage)
    target_link_options(
        child_lock_system
        PRIVATE
            --coverage)
endif()

add_executable(
    child_lock_system_tests
    tests/test_child_lock_system.cpp)

target_link_libraries(
    child_lock_system_tests
    PRIVATE
        child_lock_system
        GTest::gtest_main)

target_compile_options(
    child_lock_system_tests
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic)

if(CHILD_LOCK_ENABLE_COVERAGE)
    target_compile_options(
        child_lock_system_tests
        PRIVATE
            --coverage)
    target_link_options(
        child_lock_system_tests
        PRIVATE
            --coverage)
endif()

include(GoogleTest)
gtest_discover_tests(child_lock_system_tests)
