project(fmt_lumberyard)

target_sources(fmt_lumberyard PRIVATE SharedStructures.h)

add_subdirectory("model")
add_subdirectory("emfx")
