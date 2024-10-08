# A lot of this was copied from the JoltPhysics library
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(opal CXX)

# When turning this option on, the library will be compiled with debug symbols
option(GENERATE_DEBUG_SYMBOLS "Generate debug symbols" ON)

# Enable all warnings
option(ENABLE_ALL_WARNINGS "Enable all warnings and warnings as errors" ON)

include(CMakeDependentOption)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release;Distribution")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
    # ASAN is https://clang.llvm.org/docs/AddressSanitizer.html
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release;ReleaseASAN;ReleasgjBSAN;ReleaseCoverage;Distribution")
endif ()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows" OR "${CMAKE_SYSTEM_NAME}" STREQUAL "WindowsStore") AND NOT MINGW)
    # Fill in the path to the asan libraries
    set(CLANG_LIB_PATH "\"$(VSInstallDir)\\VC\\Tools\\Llvm\\x64\\lib\\clang\\${CMAKE_CXX_COMPILER_VERSION}\\lib\\windows\"")

    # 64 bit architecture
    set(CMAKE_VS_PLATFORM_TOOLSET_HOST_ARCHITECTURE "x64")

    # Set general compiler flags
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++20 /Zc:__cplusplus /Gm- /MP /nologo /diagnostics:classic /FC /fp:except- /Zc:inline")

    # Optionally generate debug symbols
    if (GENERATE_DEBUG_SYMBOLS)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi")
    endif ()

    # Enable warnings
    if (ENABLE_ALL_WARNINGS)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Wall /WX")
    endif ()

    # Remove any existing compiler flag that enables RTTI
    string(REPLACE "/GR" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})

    # Set compiler flag for disabling RTTI
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GR-")

    if ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "ARM")
        # On ARM the exception handling flag is missing which causes warnings
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
    endif ()

    # Set compiler flags for various configurations
    set(CMAKE_CXX_FLAGS_DEBUG "/GS /Od /Ob0 /RTC1")
    set(CMAKE_CXX_FLAGS_RELEASE "/GS- /Gy /O2 /Oi /Ot")
    set(CMAKE_CXX_FLAGS_DISTRIBUTION "/GS- /Gy /O2 /Oi /Ot")
    set(CMAKE_CXX_FLAGS_RELEASEASAN "-fsanitize=address /Od")
    set(CMAKE_CXX_FLAGS_RELEASgjBSAN "-fsanitize=undefined,implicit-conversion,float-divide-by-zero,local-bounds -fno-sanitize-recover=all")
    set(CMAKE_CXX_FLAGS_RELEASECOVERAGE "-fprofile-instr-generate -fcoverage-mapping")

    if (NOT ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "ARM64") AND NOT ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "ARM"))
        # On ARM, whole program optimization triggers an internal compiler error during code gen, so we don't turn it on
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /GL")
        set(CMAKE_CXX_FLAGS_DISTRIBUTION "${CMAKE_CXX_FLAGS_DISTRIBUTION} /GL")
    endif ()

    # Set linker flags
    set(CMAKE_EXE_LINKER_FLAGS "/ignore:4221 /DEBUG:FASTLINK") # /SUBSYSTEM:WINDOWS

    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /fp:fast") # Clang doesn't use fast math because it cannot be turned off inside a single compilation unit

        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /DJPH_FLOATING_POINT_EXCEPTIONS_ENABLED") # Clang turns Float2 into a vector sometimes causing floating point exceptions
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /DJPH_FLOATING_POINT_EXCEPTIONS_ENABLED")
        set(CMAKE_EXE_LINKER_FLAGS_RELEASE "/INCREMENTAL:NO /LTCG:incremental /OPT:ICF /OPT:REF")
        set(CMAKE_STATIC_LINKER_FLAGS_RELEASE "/LTCG")

    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /showFilenames")
        set(CMAKE_CXX_FLAGS "-Qunused-arguments ${CMAKE_CXX_FLAGS}") # Clang emits warnings about unused arguments such as /MP and /GL

        set(CMAKE_EXE_LINKER_FLAGS_RELEASEASAN "/SUBSYSTEM:CONSOLE /LIBPATH:${CLANG_LIB_PATH} clang_rt.asan-x86_64.lib -wholearchive:clang_rt.asan-x86_64.lib clang_rt.asan_cxx-x86_64.lib -wholearchive:clang_rt.asan_cxx-x86_64.lib")
        set(CMAKE_EXE_LINKER_FLAGS_RELEASgjBSAN "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /LIBPATH:${CLANG_LIB_PATH}")
        set(CMAKE_EXE_LINKER_FLAGS_RELEASECOVERAGE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /LIBPATH:${CLANG_LIB_PATH}")
    endif ()
endif ()

# Set linker flags
set(CMAKE_EXE_LINKER_FLAGS_DISTRIBUTION "${CMAKE_EXE_LINKER_FLAGS_RELEASE}")

# Set repository root
set(SUNFLOWER_REPO_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()

# Include the source directory cmake file
include(${SUNFLOWER_REPO_ROOT}/src/src.cmake)