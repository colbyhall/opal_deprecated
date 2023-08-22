# Set the root
set(HELLO_WORLD_ROOT ${PROGRAMS_ROOT}/hello_world)

# Source files
set(HELLO_WORLD_SRC_FILES
        ${HELLO_WORLD_ROOT}/hello_world.cmake
        ${HELLO_WORLD_ROOT}/main.cpp
        )

# Group source files
source_group(TREE ${HELLO_WORLD_ROOT} FILES ${HELLO_WORLD_SRC_FILES})

add_executable(hello_world WIN32 ${HELLO_WORLD_SRC_FILES})
target_include_directories(hello_world PUBLIC ${RUNTIME_ROOT})
target_link_libraries(hello_world LINK_PUBLIC core gpu editor)
# target_include_directories(playground PUBLIC ${THIRD_PARTY_ROOT})
set_target_properties(hello_world PROPERTIES FOLDER "programs")
