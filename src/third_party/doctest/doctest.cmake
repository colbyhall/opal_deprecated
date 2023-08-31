# https://github.com/doctest/doctest

# Set the root
set(DOCTEST_ROOT ${THIRD_PARTY_ROOT}/doctest)

# Source files
set(DOCTEST_SRC_FILES
	${DOCTEST_ROOT}/doctest.cmake
	${DOCTEST_ROOT}/doctest_internal.h
	${DOCTEST_ROOT}/doctest.h
	${DOCTEST_ROOT}/doctest.cpp
)

# Group source files
source_group(TREE ${DOCTEST_ROOT} FILES ${DOCTEST_SRC_FILES})

# Create Doctest lib
add_library(doctest STATIC ${DOCTEST_SRC_FILES})
target_link_libraries(doctest LINK_PUBLIC core)
target_include_directories(doctest PRIVATE ${RUNTIME_ROOT} ${THIRD_PARTY_ROOT})
set_target_properties(doctest PROPERTIES FOLDER "third_party")
# set_target_properties(doctest PROPERTIES
# 	VS_GLOBAL_RunCodeAnalysis true

# 	 # Use clangtidy
# 	 VS_GLOBAL_EnableClangTidyCodeAnalysis true
# 	 VS_GLOBAL_ClangTidyChecks -checks=-*,modernize-*,-modernize-use-trailing-return-type
# )
