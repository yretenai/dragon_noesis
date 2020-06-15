project(TOOL_lumberyard_test)
add_executable(lumberyard_test main.cpp)
target_link_libraries(lumberyard_test dragon_lumberyard)

if(USE_NOESIS)
    add_compile_definitions(USE_NOESIS)
endif()
