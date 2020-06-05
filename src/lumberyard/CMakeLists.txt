project(fmt_lumberyard)

add_library(fmt_lumberyard SHARED
            Lumberyard.cpp Lumberyard.h
            Model.cpp Model.h
            Animation.cpp Animation.h
            Actor.cpp Actor.h
            Texture.cpp Texture.h)
target_link_libraries(fmt_lumberyard fmt_dragon)

if(${USE_NOESIS})
    target_sources(fmt_lumberyard PRIVATE ../noesis/noesisplugin.cpp ../noesis/pluginsupport.cpp)
    add_compile_definitions(USE_NOESIS)
endif()

if(${MSVC})
    generate_export_header(fmt_lumberyard BASE_NAME LUMBERYARD DEFINE_NO_DEPRECATED EXPORT_FILE_NAME ${PROJECT_SOURCE_DIR}/export_win32.h)
else()
    generate_export_header(fmt_lumberyard BASE_NAME LUMBERYARD DEFINE_NO_DEPRECATED EXPORT_FILE_NAME ${PROJECT_SOURCE_DIR}/export_clang.h)
endif()

if(${USE_NOESIS})
    install(TARGETS fmt_lumberyard
            RUNTIME DESTINATION "plugins"
            COMPONENT RUNTIME)
endif()

add_subdirectory("chunks")
