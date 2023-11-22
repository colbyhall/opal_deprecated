# Set the root
set(GAME_ROOT ${RUNTIME_ROOT}/game)

# Source files
set(GAME_SRC_FILES
        ${GAME_ROOT}/archetype.h
        ${GAME_ROOT}/component.h
        ${GAME_ROOT}/entity.h
        ${GAME_ROOT}/game.cmake
        ${GAME_ROOT}/game.h
        ${GAME_ROOT}/storage.h
        ${GAME_ROOT}/world.h
)

# Group source files
source_group(TREE ${GAME_ROOT} FILES ${GAME_SRC_FILES})

# Create GAME lib
add_library(game STATIC ${GAME_SRC_FILES})
target_link_libraries(game LINK_PUBLIC core)

target_include_directories(game PUBLIC ${RUNTIME_ROOT})
set_target_properties(game PROPERTIES FOLDER "runtime")
