project(dragon_warofdragon)

add_library(dragon_warofdragon SHARED
            WarOfDragon.cpp WarOfDragon.h
            FileTable.cpp FileTable.h)

target_compile_definitions(dragon_warofdragon PRIVATE DRAGON_LIBRARY_NAME="dragon_warofdragon")

if(USE_NOESIS)
    target_compile_definitions(dragon_lumberyard PRIVATE USE_NOESIS=1)
    target_sources(dragon_warofdragon PRIVATE ${NOESIS_API_SOURCES} FileTable_Noesis.cpp)
    add_compile_definitions(USE_NOESIS)
endif()

if(MSVC)
    generate_export_header(dragon_warofdragon BASE_NAME WAROFDRAGON DEFINE_NO_DEPRECATED EXPORT_FILE_NAME ${PROJECT_SOURCE_DIR}/export_win32.h)
else()
    generate_export_header(dragon_warofdragon BASE_NAME WAROFDRAGON DEFINE_NO_DEPRECATED EXPORT_FILE_NAME ${PROJECT_SOURCE_DIR}/export_clang.h)
endif()

if(USE_NOESIS)
    install(TARGETS dragon_warofdragon EXPORT dragon_warofdragon
            RUNTIME DESTINATION "plugins"
            COMPONENT dragon_warofdragon)
endif()
