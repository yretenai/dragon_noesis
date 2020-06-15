project(fmt_warofdragon)

add_library(fmt_warofdragon SHARED
            WarOfDragon.cpp WarOfDragon.h
            FileTable.cpp FileTable.h)

target_compile_definitions(fmt_warofdragon PRIVATE DRAGON_LIBRARY_NAME="fmt_warofdragon")

if(USE_NOESIS)
    target_sources(fmt_warofdragon PRIVATE ../noesis/noesisplugin.cpp ../noesis/pluginsupport.cpp FileTable_Noesis.cpp)
    add_compile_definitions(USE_NOESIS)
endif()

if(MSVC)
    generate_export_header(fmt_warofdragon BASE_NAME LUMBERYARD DEFINE_NO_DEPRECATED EXPORT_FILE_NAME ${PROJECT_SOURCE_DIR}/export_win32.h)
else()
    generate_export_header(fmt_warofdragon BASE_NAME LUMBERYARD DEFINE_NO_DEPRECATED EXPORT_FILE_NAME ${PROJECT_SOURCE_DIR}/export_clang.h)
endif()

if(USE_NOESIS)
    install(TARGETS fmt_warofdragon
            RUNTIME DESTINATION "plugins"
            COMPONENT RUNTIME)
endif()
