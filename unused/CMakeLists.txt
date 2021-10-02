#
# === CMake lists for the mvisus ===
#

# === CMake options ===

# Set CMake version
cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

option(SUPERBUILD "Build libraries and mvisus project" OFF)

if (SUPERBUILD)
    include(superbuild.cmake)
else()
    include(mvisus.cmake)
endif()
