cmake_minimum_required(VERSION 3.16)

# Top CMake script for building PAF.
# Author: Igor Lesik 2022

project(PAF
    VERSION 0.0.1
    DESCRIPTION "Play Audio File application"
    LANGUAGES ASM C CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_C_FLAGS "-O2 -Wall -Wextra")
    set(CMAKE_CXX_FLAGS "-O2 -Wall -Wextra")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_C_FLAGS "/O2 /Wall")
    set(CMAKE_CXX_FLAGS "/O2 /Wall")
#elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
#elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
endif()

include_directories(.)

# Install inside the build directory.
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install)

#enable_testing()

add_subdirectory(JUCE) # we have JUCE as git submodule
add_subdirectory(paf)
#add_subdirectory(test)
