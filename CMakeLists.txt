cmake_minimum_required(VERSION 3.13)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "")
endif()
message(STATUS "CMAKE_EXPORT_COMPILE_COMMANDS: ${CMAKE_EXPORT_COMPILE_COMMANDS}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENTIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

project(cdscript CXX)

find_package(Catch2 CONFIG REQUIRED)

add_library(cdscript STATIC src/lexer.cpp)
add_executable(test src_test/test.cpp)
target_link_libraries(test PRIVATE Catch2::Catch2 PRIVATE cdscript)
target_include_directories(test PRIVATE src)