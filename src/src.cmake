# Set the root
set(SRC_ROOT ${EUPHORIA_REPO_ROOT}/src)

# Set the programs root
set(PROGRAMS_ROOT ${SRC_ROOT}/programs)

# Set the runtime root
set(RUNTIME_ROOT ${SRC_ROOT}/runtime)

# Set the test root
# set(TEST_ROOT ${SRC_ROOT}/test)

# Set the third_party root
# set(THIRD_PARTY_ROOT ${SRC_ROOT}/third_party)

# Enable folders for visual studio
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include(${SRC_ROOT}/programs/programs.cmake)
include(${SRC_ROOT}/runtime/runtime.cmake)
# include(${SRC_ROOT}/third_party/third_party.cmake)
