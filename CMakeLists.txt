cmake_minimum_required(VERSION 3.12)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "")
endif()
message(STATUS "CMAKE_EXPORT_COMPILE_COMMANDS: ${CMAKE_EXPORT_COMPILE_COMMANDS}")
message(STATUS "CMAKE_C_COMPILER: ${CMAKE_C_COMPILER}")
message(STATUS "CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")
option(ENABLE_COVERAGE "Enable Coverage Reporting" FALSE)
message(STATUS "ENABLE_COVERAGE: ${ENABLE_COVERAGE}")

project(cdscript CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENTIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX  /permissive-")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Werror")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Werror")
    if(ENABLE_COVERAGE)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage --coverage")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
    endif()
endif()

find_package(Catch2 CONFIG REQUIRED)

add_library(cdscript STATIC 
src/lexer.cpp
src/syntax.cpp
src/parser.cpp
)
set(TEST_SOURCE_LIST
src_test/catch2_ext.hpp
src_test/test_lexer_comment.cpp
src_test/test_lexer_identifier.cpp
src_test/test_lexer_newline.cpp
src_test/test_lexer_number.cpp
src_test/test_lexer_simple.cpp
src_test/test_lexer_string.cpp
src_test/test_parser.cpp
src_test/test_serialize.cpp
src_test/test_token_number.cpp
src_test/test.cpp
)
add_executable(unittest ${TEST_SOURCE_LIST})
target_include_directories(unittest PRIVATE src)
target_link_libraries(unittest PRIVATE Catch2::Catch2 PRIVATE cdscript)

enable_testing()
add_test(NAME unittest COMMAND unittest)