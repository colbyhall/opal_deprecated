# Set the root
set(CORE_ROOT ${RUNTIME_ROOT}/core)

# Source files
set(CORE_SRC_FILES
	${CORE_ROOT}/core.cmake
	${CORE_ROOT}/core.cpp
	${CORE_ROOT}/core.h
	${CORE_ROOT}/core.natvis
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
set_target_properties(core PROPERTIES
	VS_GLOBAL_RunCodeAnalysis true

	 # Use clangtidy
	 VS_GLOBAL_EnableClangTidyCodeAnalysis true
	 VS_GLOBAL_ClangTidyChecks -checks=-*,modernize-*,-modernize-use-trailing-return-type
)
