# Set the root
set(CORE_ROOT ${RUNTIME_ROOT}/core)

# Source files
set(CORE_SRC_FILES
        ${CORE_ROOT}/containers/array.h
        ${CORE_ROOT}/containers/array.inl
        ${CORE_ROOT}/containers/function.h
        ${CORE_ROOT}/containers/function.inl
        ${CORE_ROOT}/containers/map.h
        ${CORE_ROOT}/containers/map.inl
        ${CORE_ROOT}/containers/non_null.h
        ${CORE_ROOT}/containers/option.h
        ${CORE_ROOT}/containers/result.h
        ${CORE_ROOT}/containers/shared.h
        ${CORE_ROOT}/containers/shared.inl
        ${CORE_ROOT}/containers/slice.h
        ${CORE_ROOT}/containers/slice.inl
        ${CORE_ROOT}/containers/string_view.h
        ${CORE_ROOT}/containers/string_view.cpp
        ${CORE_ROOT}/containers/string.h
        ${CORE_ROOT}/containers/string.cpp
        ${CORE_ROOT}/containers/tuple.h
        ${CORE_ROOT}/containers/unique.h
        ${CORE_ROOT}/containers/unique.inl
        ${CORE_ROOT}/containers/vector.h
        ${CORE_ROOT}/containers/vector.inl
        ${CORE_ROOT}/containers/wstring.h
        ${CORE_ROOT}/containers/wstring.cpp
        ${CORE_ROOT}/containers/wstring_view.h

        ${CORE_ROOT}/math/aabb2.h
        ${CORE_ROOT}/math/aabb2.inl
        ${CORE_ROOT}/math/math.h
        ${CORE_ROOT}/math/math.cpp
        ${CORE_ROOT}/math/matrix4.h
        ${CORE_ROOT}/math/matrix4.inl
        ${CORE_ROOT}/math/quaternion.h
        ${CORE_ROOT}/math/quaternion.inl
        ${CORE_ROOT}/math/vector2.h
        ${CORE_ROOT}/math/vector2.inl
        ${CORE_ROOT}/math/vector3.h
        ${CORE_ROOT}/math/vector3.inl
        ${CORE_ROOT}/math/vector4.h
        ${CORE_ROOT}/math/vector4.inl

        ${CORE_ROOT}/os/file_system.h
        ${CORE_ROOT}/os/file_system.cpp
        ${CORE_ROOT}/os/library.h
        ${CORE_ROOT}/os/library.cpp
        ${CORE_ROOT}/os/memory.h
        ${CORE_ROOT}/os/memory.cpp
        ${CORE_ROOT}/os/windows.h

        ${CORE_ROOT}/atomic.h
        ${CORE_ROOT}/atomic.inl
        ${CORE_ROOT}/core.cmake
        ${CORE_ROOT}/core.cpp
        ${CORE_ROOT}/core.h
        ${CORE_ROOT}/core.natvis
        ${CORE_ROOT}/hash.h
        ${CORE_ROOT}/hash.cpp
        ${CORE_ROOT}/initializer_list.h
        ${CORE_ROOT}/non_copyable.h
        ${CORE_ROOT}/type_traits.h
)

if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    # Add natvis file
    set(CORE_SRC_FILES ${CORE_SRC_FILES} ${CORE_ROOT}/core.natvis)
endif ()

# Group source files
source_group(TREE ${CORE_ROOT} FILES ${CORE_SRC_FILES})

# Create Core lib
add_library(core STATIC ${CORE_SRC_FILES})
target_include_directories(core PUBLIC ${RUNTIME_ROOT})
set_target_properties(core PROPERTIES FOLDER "runtime")
