# https://github.com/nothings/stb

# Set the root
set(STB_ROOT ${THIRD_PARTY_ROOT}/stb)

# Source files
set(STB_SRC_FILES
        ${STB_ROOT}/stb.cmake
		${STB_ROOT}/stb.h
        ${STB_ROOT}/stb_image.h
        ${STB_ROOT}/stb_image.cpp
        )

# Group source files
source_group(TREE ${STB_ROOT} FILES ${STB_SRC_FILES})


# Create STB lib
add_library(stb STATIC ${STB_SRC_FILES})
target_link_libraries(stb LINK_PUBLIC core)
target_include_directories(stb PRIVATE ${RUNTIME_ROOT} ${THIRD_PARTY_ROOT})
set_target_properties(stb PROPERTIES FOLDER "third_party")

# Disable warnings are errors for clang
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(stb PRIVATE -Wno-error)
endif()
# And similarly for MSVC
if(MSVC)
    target_compile_options(stb PRIVATE /WX-)
endif()
