# Set the root
set(IMGUI_ROOT ${THIRD_PARTY_ROOT}/imgui)

# Source files
set(IMGUI_SRC_FILES
        ${IMGUI_ROOT}/imgui.cmake
        ${IMGUI_ROOT}/imgui.h
        ${IMGUI_ROOT}/imgui.cpp
        ${IMGUI_ROOT}/imgui_draw.cpp
        ${IMGUI_ROOT}/imgui_internal.h
        ${IMGUI_ROOT}/imgui_tables.cpp
        ${IMGUI_ROOT}/imgui_widgets.cpp
        ${IMGUI_ROOT}/imstb_rectpack.h
        ${IMGUI_ROOT}/imstb_textedit.h
        ${IMGUI_ROOT}/imstb_truetype.h
        )

# Group source files
source_group(TREE ${IMGUI_ROOT} FILES ${IMGUI_SRC_FILES})


# Create Imgui lib
add_library(imgui STATIC ${IMGUI_SRC_FILES})
target_link_libraries(imgui LINK_PUBLIC core)
target_include_directories(imgui PRIVATE ${RUNTIME_ROOT} ${IMGUI_ROOT})
set_target_properties(imgui PROPERTIES FOLDER "third_party")

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(imgui PRIVATE -Wno-error)
endif()

# And similarly for MSVC
if(MSVC)
    target_compile_options(imgui PRIVATE /WX-)
endif()