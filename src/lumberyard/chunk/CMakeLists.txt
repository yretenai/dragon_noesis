project(dragon_lumberyard)

target_sources(dragon_lumberyard PRIVATE SharedStructures.h)

add_subdirectory("model")
add_subdirectory("emfx")
