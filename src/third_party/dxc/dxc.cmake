# Set the root
set(DXC_ROOT ${THIRD_PARTY_ROOT}/dxc)

# Source files
set(DXC_SRC_FILES
        ${DXC_ROOT}/dxc.cmake
        ${DXC_ROOT}/dxc.h
        ${DXC_ROOT}/dxc.cpp
        )

# Group source files
source_group(TREE ${DXC_ROOT} FILES ${DXC_SRC_FILES})

# Create dxc lib
add_library(dxc STATIC ${DXC_SRC_FILES})
target_include_directories(dxc PUBLIC ${RUNTIME_ROOT} ${THIRD_PARTY_ROOT})
target_link_libraries(dxc LINK_PUBLIC core gpu ${DXC_ROOT}/lib/x64/dxcompiler.lib)
set_target_properties(dxc PROPERTIES FOLDER "third_party")

add_custom_command(
        TARGET dxc POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${DXC_ROOT}/bin/x64/dxcompiler.dll
        ${CMAKE_CURRENT_BINARY_DIR}/dxcompiler.dll
)

add_custom_command(
        TARGET dxc POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${DXC_ROOT}/bin/x64/dxil.dll
        ${CMAKE_CURRENT_BINARY_DIR}/dxil.dll
)
