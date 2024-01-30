########################################################################
# Project configuration
########################################################################

cmake_minimum_required(VERSION 2.8.11)

project(Ealain)

# The project version number.
set(VERSION_MAJOR   0   CACHE STRING "Project major version number.")
set(VERSION_MINOR   1   CACHE STRING "Project minor version number.")
set(VERSION_PATCH   1   CACHE STRING "Project patch version number.")
mark_as_advanced(VERSION_MAJOR VERSION_MINOR VERSION_PATCH)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")

# Put binaries in the build directory root.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Dump used compiler flags (needed by some IDEs).
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Specify the cross-compiler
#set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
#set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

########################################################################
# Build options
########################################################################

# Make sure the compiler can find include files
# when other libraries or executables link
include_directories(.)

# Do not disable superfluous warnings by default.
option(USE_LIB_LEATHERS "Disable some useless compilation warnings" OFF)
if(USE_LIB_LEATHERS)
    include_directories(utils/leathers/Source/)
    add_definitions(-DWITH_LEATHERS)
endif()

# (Disabled) Do not replace traditional asserts by default.
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    option(USE_THROWN_ASSERTS "Replace traditional asserts with ones throwing exceptions" OFF)
    if(USE_THROWN_ASSERTS)
        add_definitions(-DWITH_THROWN_ASSERTS)
    endif()
endif()

########################################################################
# Build commands.
########################################################################

# Library.
add_subdirectory(Ealain)
enable_testing()
add_subdirectory(test)
add_subdirectory(drift)
add_subdirectory(mo)
add_subdirectory(so)
add_subdirectory(co)
add_subdirectory(mf)
