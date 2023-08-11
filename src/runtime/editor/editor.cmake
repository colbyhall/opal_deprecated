# Set the root
set(EDITOR_ROOT ${RUNTIME_ROOT}/EDITOR)

# Source files
set(EDITOR_SRC_FILES
        ${EDITOR_ROOT}/application.h
        ${EDITOR_ROOT}/application.cpp
        ${EDITOR_ROOT}/editor.cmake
        ${EDITOR_ROOT}/editor.h
        )

# Group source files
source_group(TREE ${EDITOR_ROOT} FILES ${EDITOR_SRC_FILES})

# Create editor lib
add_library(editor STATIC ${EDITOR_SRC_FILES})
target_link_libraries(editor LINK_PUBLIC core gpu imgui dxc)

target_include_directories(editor PUBLIC ${RUNTIME_ROOT} ${THIRD_PARTY_ROOT})
set_target_properties(editor PROPERTIES FOLDER "runtime")
