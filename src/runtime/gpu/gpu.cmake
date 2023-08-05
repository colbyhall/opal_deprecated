# Set the root
set(GPU_ROOT ${RUNTIME_ROOT}/gpu)

# Source files
set(GPU_SRC_FILES
        ${GPU_ROOT}/d3d12/d3d12.h
        ${GPU_ROOT}/d3d12/d3d12_buffer.h
        ${GPU_ROOT}/d3d12/d3d12_buffer.cpp
        ${GPU_ROOT}/d3d12/d3d12_context.h
        ${GPU_ROOT}/d3d12/d3d12_context.cpp
        ${GPU_ROOT}/d3d12/d3d12_memory.h
        ${GPU_ROOT}/d3d12/d3d12_memory.cpp
        ${GPU_ROOT}/d3d12/d3d12_graphics_command_list.h
        ${GPU_ROOT}/d3d12/d3d12_graphics_command_list.cpp
        ${GPU_ROOT}/d3d12/d3d12_graphics_pipeline.h
        ${GPU_ROOT}/d3d12/d3d12_graphics_pipeline.cpp
        ${GPU_ROOT}/d3d12/d3d12_shader.h
        ${GPU_ROOT}/d3d12/d3d12_texture.h
        ${GPU_ROOT}/d3d12/d3d12_texture.cpp

        ${GPU_ROOT}/buffer.h
        ${GPU_ROOT}/buffer.cpp
        ${GPU_ROOT}/context.h
        ${GPU_ROOT}/context.cpp
        ${GPU_ROOT}/gpu.cmake
        ${GPU_ROOT}/gpu.h
        ${GPU_ROOT}/graphics_command_list.h
        ${GPU_ROOT}/graphics_command_list.cpp
        ${GPU_ROOT}/graphics_pipeline.h
        ${GPU_ROOT}/graphics_pipeline.cpp
        ${GPU_ROOT}/shader.h
        ${GPU_ROOT}/shader.cpp
        ${GPU_ROOT}/texture.h
        ${GPU_ROOT}/texture.cpp
        )

# Group source files
source_group(TREE ${GPU_ROOT} FILES ${GPU_SRC_FILES})

# Create GPU lib
add_library(gpu STATIC ${GPU_SRC_FILES})
target_link_libraries(gpu LINK_PUBLIC core)

# On Windows, Link DirectX Graphics Infrastructure to load Direct3D12
if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    target_link_libraries(gpu LINK_PUBLIC "dxgi.dll")
endif ()


target_include_directories(gpu PUBLIC ${RUNTIME_ROOT})
set_target_properties(gpu PROPERTIES FOLDER "runtime")
