cmake_minimum_required(VERSION 3.16)
project(fmt_dragon)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(${MSVC})
    # warning C4068: unknown pragma
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -wd\"4068\"")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    if(NOT DEFINED CMAKE_CXX_COMPILER)
        set(CMAKE_CXX_COMPILER "clang++-10")
    endif()
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++abi")
endif()

set(USE_NOESIS off CACHE BOOL "Use and Export Noesis functions")

include(GenerateExportHeader)

add_library(fmt_dragon SHARED dragon.cpp)
if(${MSVC})
    generate_export_header(fmt_dragon BASE_NAME DRAGON DEFINE_NO_DEPRECATED EXPORT_FILE_NAME ${PROJECT_SOURCE_DIR}/export_win32.h)
else()
    generate_export_header(fmt_dragon BASE_NAME DRAGON DEFINE_NO_DEPRECATED EXPORT_FILE_NAME ${PROJECT_SOURCE_DIR}/export_clang.h)
endif()

add_library(fmt_lumberyard SHARED
        lumberyard/Model.cpp lumberyard/Animation.cpp lumberyard/Actor.cpp
        lumberyard/Texture.cpp lumberyard/Lumberyard.cpp lumberyard/chunks/model/ExportFlags.cpp
        lumberyard/chunks/model/MaterialName.cpp lumberyard/chunks/model/Submesh.cpp lumberyard/chunks/model/DataStream.cpp lumberyard/chunks/model/DataStream.h)
target_link_libraries(fmt_lumberyard fmt_dragon)

if(${USE_NOESIS})
    target_sources(fmt_lumberyard PRIVATE noesis/noesisplugin.cpp noesis/pluginsupport.cpp)
    add_compile_definitions(USE_NOESIS)
endif()

if(${MSVC})
    generate_export_header(fmt_lumberyard BASE_NAME LUMBERYARD DEFINE_NO_DEPRECATED EXPORT_FILE_NAME ${PROJECT_SOURCE_DIR}/lumberyard/export_win32.h)
else()
    generate_export_header(fmt_lumberyard BASE_NAME LUMBERYARD DEFINE_NO_DEPRECATED EXPORT_FILE_NAME ${PROJECT_SOURCE_DIR}/lumberyard/export_clang.h)
endif()

add_executable(lumberyard_test tool/lumberyard/main.cpp)
target_link_libraries(lumberyard_test fmt_lumberyard fmt_dragon)

if(${USE_NOESIS})
install(TARGETS fmt_lumberyard
        RUNTIME DESTINATION "plugins"
        COMPONENT RUNTIME)
endif()
