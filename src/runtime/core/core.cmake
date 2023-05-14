# Set the root
set(CORE_ROOT ${RUNTIME_ROOT}/core)

# Source files
set(CORE_SRC_FILES
	${CORE_ROOT}/core.cmake
	${CORE_ROOT}/core.cpp
	${CORE_ROOT}/core.h
	${CORE_ROOT}/core.natvis
	${CORE_ROOT}/initializer_list.h
	${CORE_ROOT}/memory.h
	${CORE_ROOT}/memory.cpp
	${CORE_ROOT}/non_copyable.h
	${CORE_ROOT}/type_traits.h
	${CORE_ROOT}/containers/array.h
	${CORE_ROOT}/containers/array.inl
	${CORE_ROOT}/containers/non_null.h
	${CORE_ROOT}/containers/option.h
	${CORE_ROOT}/containers/slice.h
	${CORE_ROOT}/containers/slice.inl
)

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
	# Add natvis file
	set(CORE_SRC_FILES ${CORE_SRC_FILES} ${CORE_ROOT}/core.natvis)
endif()

# Group source files
source_group(TREE ${CORE_ROOT} FILES ${CORE_SRC_FILES})

# Create Core lib
add_library(core STATIC ${CORE_SRC_FILES})
target_include_directories(core PUBLIC ${RUNTIME_ROOT})
set_target_properties(core PROPERTIES FOLDER "runtime")
