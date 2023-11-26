# Set the root
set(GUI_ROOT ${RUNTIME_ROOT}/gui)

# Source files
set(GUI_SRC_FILES
        ${GUI_ROOT}/application.h
        ${GUI_ROOT}/application.cpp
        ${GUI_ROOT}/builder.h
        ${GUI_ROOT}/builder.cpp
        ${GUI_ROOT}/canvas.h
        ${GUI_ROOT}/canvas.cpp
        ${GUI_ROOT}/gui.cmake
        ${GUI_ROOT}/gui.h
        ${GUI_ROOT}/widgets/label.h
        ${GUI_ROOT}/widgets/label.cpp
)

# Group source files
source_group(TREE ${GUI_ROOT} FILES ${GUI_SRC_FILES})

# Create GUI lib
add_library(gui STATIC ${GUI_SRC_FILES})
target_link_libraries(gui LINK_PUBLIC core gpu dxc)
target_include_directories(gui PUBLIC ${RUNTIME_ROOT})
target_include_directories(gui PUBLIC ${THIRD_PARTY})
set_target_properties(gui PROPERTIES FOLDER "runtime")
