cmake_minimum_required(VERSION 3.20)

set(CMAKE_C_COMPILER gcc-11 CACHE STRING "Pinned C compiler")
set(CMAKE_CXX_COMPILER g++-11 CACHE STRING "Pinned C++ compiler")

project(
    MBP_T02_ChildLock
    VERSION 1.0
    DESCRIPTION "UC01 Electronic Child Lock System"
    LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(CHILD_LOCK_ENABLE_COVERAGE "Enable gcov coverage flags" OFF)
option(USE_SYSTEM_GTEST "Use preinstalled GoogleTest if available." ON)
set(GTEST_SOURCE_DIR "" CACHE PATH "Optional local path to googletest source.")

file(GLOB_RECURSE CHILD_LOCK_HEADERS CONFIGURE_DEPENDS
    ${PROJECT_SOURCE_DIR}/include/*.h)
file(GLOB_RECURSE CHILD_LOCK_SOURCES CONFIGURE_DEPENDS
    ${PROJECT_SOURCE_DIR}/src/*.c)
file(GLOB_RECURSE CHILD_LOCK_CPP_TEST_SOURCES CONFIGURE_DEPENDS
    ${PROJECT_SOURCE_DIR}/tests/*.cpp)
file(GLOB_RECURSE CHILD_LOCK_C_TEST_SOURCES CONFIGURE_DEPENDS
    ${PROJECT_SOURCE_DIR}/tests/*.c)

if(MSVC)
    set(CHILD_LOCK_STRICT_WARNINGS /W4 /WX)
else()
    set(CHILD_LOCK_STRICT_WARNINGS -Wall -Wextra -Werror -Wpedantic)
endif()

add_library(
    child_lock_system
    ${CHILD_LOCK_SOURCES}
    ${CHILD_LOCK_HEADERS})

target_include_directories(
    child_lock_system
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include)

target_compile_options(
    child_lock_system
    PRIVATE
        ${CHILD_LOCK_STRICT_WARNINGS})

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

enable_testing()

if(CHILD_LOCK_CPP_TEST_SOURCES)
    if(USE_SYSTEM_GTEST)
        find_package(GTest QUIET)
    endif()

    if(NOT GTest_FOUND)
        include(FetchContent)
        if(GTEST_SOURCE_DIR)
            FetchContent_Declare(
                googletest
                SOURCE_DIR ${GTEST_SOURCE_DIR})
        else()
            FetchContent_Declare(
                googletest
                URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
                DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
        endif()
        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
        FetchContent_MakeAvailable(googletest)
    endif()

    add_executable(
        child_lock_system_tests
        ${CHILD_LOCK_CPP_TEST_SOURCES})

    if(GTest_FOUND)
        target_link_libraries(
            child_lock_system_tests
            PRIVATE
                child_lock_system
                GTest::gtest_main)
    else()
        target_link_libraries(
            child_lock_system_tests
            PRIVATE
                child_lock_system
                gtest_main)
    endif()

    target_compile_options(
        child_lock_system_tests
        PRIVATE
            ${CHILD_LOCK_STRICT_WARNINGS})

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
endif()

foreach(child_lock_c_test_source IN LISTS CHILD_LOCK_C_TEST_SOURCES)
    get_filename_component(child_lock_c_test_name
        ${child_lock_c_test_source}
        NAME_WE)
    set(child_lock_c_test_target ${child_lock_c_test_name}_c_tests)

    add_executable(
        ${child_lock_c_test_target}
        ${child_lock_c_test_source})
    target_link_libraries(
        ${child_lock_c_test_target}
        PRIVATE
            child_lock_system)
    target_include_directories(
        ${child_lock_c_test_target}
        PRIVATE
            ${PROJECT_SOURCE_DIR}/include)
    target_compile_options(
        ${child_lock_c_test_target}
        PRIVATE
            ${CHILD_LOCK_STRICT_WARNINGS})

    if(CHILD_LOCK_ENABLE_COVERAGE)
        target_compile_options(
            ${child_lock_c_test_target}
            PRIVATE
                --coverage)
        target_link_options(
            ${child_lock_c_test_target}
            PRIVATE
                --coverage)
    endif()

    add_test(
        NAME ${child_lock_c_test_target}
        COMMAND ${child_lock_c_test_target})
endforeach()
