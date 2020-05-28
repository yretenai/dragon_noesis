cmake_minimum_required(VERSION 3.16)
project(fmt_dragon)

set(CMAKE_CXX_STANDARD 17)
if(${MSVC})
    # warning C4068: unknown pragma
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -wd\"4068\"")
endif()

set(USE_NOESIS off CACHE BOOL "Use and Export Noesis functions")

include(GenerateExportHeader)

add_library(fmt_dragon STATIC dragon.cpp)

add_library(fmt_lumberyard SHARED
        lumberyard/LumberyardModel.cpp lumberyard/LumberyardAnimation.cpp lumberyard/LumberyardActor.cpp
        lumberyard/LumberyardTexture.cpp lumberyard/LumberyardTexture.h lumberyard/Lumberyard.cpp)
target_link_libraries(fmt_lumberyard fmt_dragon)

if(${USE_NOESIS})
    target_sources(fmt_lumberyard PRIVATE noesis/noesisplugin.cpp noesis/pluginsupport.cpp)
    add_compile_definitions(USE_NOESIS)
endif()

generate_export_header(fmt_lumberyard BASE_NAME DRAGON DEFINE_NO_DEPRECATED EXPORT_FILE_NAME ${PROJECT_SOURCE_DIR}/lumberyard/export.h)

add_executable(lumberyard_test tool/lumberyard/main.cpp)
target_link_libraries(lumberyard_test fmt_lumberyard)

install(TARGETS fmt_lumberyard
        RUNTIME DESTINATION "plugins"
        COMPONENT RUNTIME)
