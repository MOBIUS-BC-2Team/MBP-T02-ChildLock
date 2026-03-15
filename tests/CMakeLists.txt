cmake_minimum_required(VERSION 3.20)

project(MBP_T02_ChildLock_Tests C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
  add_compile_options(/W4 /WX)
else()
  add_compile_options(-Wall -Wextra -Werror)
endif()

option(USE_SYSTEM_GTEST "Use preinstalled GoogleTest if available." ON)
set(GTEST_SOURCE_DIR "" CACHE PATH "Optional local path to googletest source.")

if(USE_SYSTEM_GTEST)
  find_package(GTest QUIET)
endif()

if(NOT GTest_FOUND)
  include(FetchContent)
  if(GTEST_SOURCE_DIR)
    FetchContent_Declare(
      googletest
      SOURCE_DIR ${GTEST_SOURCE_DIR}
    )
  else()
    FetchContent_Declare(
      googletest
      URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
      DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
  endif()
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)
endif()

add_library(childlock
  ${CMAKE_CURRENT_SOURCE_DIR}/../src/control_rear_passenger_door.c
)

target_include_directories(childlock PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

add_executable(test_control_rear_passenger_door
  ${CMAKE_CURRENT_SOURCE_DIR}/test_control_rear_passenger_door.cpp
)

if(GTest_FOUND)
  target_link_libraries(test_control_rear_passenger_door
    childlock
    GTest::gtest_main
  )
else()
  target_link_libraries(test_control_rear_passenger_door
    childlock
    gtest_main
  )
endif()

include(GoogleTest)
enable_testing()
gtest_discover_tests(test_control_rear_passenger_door)
