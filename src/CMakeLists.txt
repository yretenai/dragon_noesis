cmake_minimum_required(VERSION 3.16)
project(fmt_dragon)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(${MSVC})
    # warning C4068: unknown pragma
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -wd\"4068\" -wd\"4996\" -wd\"4251\"")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++abi")
endif()

set(USE_NOESIS OFF CACHE BOOL "Use and Export Noesis functions")

include(GenerateExportHeader)

add_library(fmt_dragon SHARED dragon.cpp dragon.h Array.h)
if(${MSVC})
    generate_export_header(fmt_dragon BASE_NAME DRAGON DEFINE_NO_DEPRECATED EXPORT_FILE_NAME ${PROJECT_SOURCE_DIR}/export_win32.h)
else()
    generate_export_header(fmt_dragon BASE_NAME DRAGON DEFINE_NO_DEPRECATED EXPORT_FILE_NAME ${PROJECT_SOURCE_DIR}/export_clang.h)
endif()

add_subdirectory("lumberyard")
add_subdirectory("tool")
