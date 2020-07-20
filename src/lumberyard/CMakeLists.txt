project(dragon_lumberyard)

add_library(dragon_lumberyard SHARED
            ../vendor/tinyxml2/tinyxml2.cpp # tinyxml needs to be statically linked.
            Lumberyard.cpp Lumberyard.h
            Model.cpp Model.h
            Animation.cpp Animation.h
            Actor.cpp Actor.h
            Texture.cpp Texture.h
            Material.cpp Material.h globals.h)

target_compile_definitions(dragon_lumberyard PRIVATE DRAGON_LIBRARY_NAME="dragon_lumberyard")

if(USE_NOESIS)
    target_compile_definitions(dragon_lumberyard PRIVATE USE_NOESIS=1)
    target_sources(dragon_lumberyard PRIVATE ${NOESIS_API_SOURCES}
                   Model_Noesis.cpp Actor_Noesis.cpp Texture_Noesis.cpp Animation_Noesis.cpp)
    add_compile_definitions(USE_NOESIS)
endif()

if(MSVC)
    generate_export_header(dragon_lumberyard BASE_NAME LUMBERYARD DEFINE_NO_DEPRECATED EXPORT_FILE_NAME ${PROJECT_SOURCE_DIR}/export_win32.h)
else()
    generate_export_header(dragon_lumberyard BASE_NAME LUMBERYARD DEFINE_NO_DEPRECATED EXPORT_FILE_NAME ${PROJECT_SOURCE_DIR}/export_clang.h)
endif()

if(USE_NOESIS)
    install(TARGETS dragon_lumberyard EXPORT dragon_lumberyard
            RUNTIME DESTINATION "plugins"
            COMPONENT dragon_lumberyard)
endif()

add_subdirectory("chunk")
