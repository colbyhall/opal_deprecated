# Set the root
set(CORE_TEST_ROOT ${TEST_ROOT}/core_test)

# Source files
set(CORE_TEST_SRC_FILES
	${CORE_TEST_ROOT}/core_test.cmake
	${CORE_TEST_ROOT}/core_test.cpp
	${CORE_TEST_ROOT}/memory_test.cpp
	${CORE_TEST_ROOT}/containers/array_test.cpp
	${CORE_TEST_ROOT}/containers/non_null_test.cpp
	${CORE_TEST_ROOT}/containers/option_test.cpp
	${CORE_TEST_ROOT}/containers/slice_test.cpp
)

# Group source files
source_group(TREE ${CORE_TEST_ROOT} FILES ${CORE_TEST_SRC_FILES})

add_executable(core_test ${CORE_TEST_SRC_FILES})
target_include_directories(core_test PUBLIC ${RUNTIME_ROOT} ${THIRD_PARTY_ROOT})
target_link_libraries(core_test LINK_PUBLIC core doctest)
set_target_properties(core_test PROPERTIES FOLDER "test")

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows" AND NOT MINGW)
	target_link_options(core_test PUBLIC "/SUBSYSTEM:CONSOLE")
endif()

enable_testing()
add_test(core_test core_test)
