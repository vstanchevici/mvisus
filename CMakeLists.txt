# 
# CMake options
# 

# CMake version
cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

#
# Configure CMake environment
#

# Register general cmake commands
#include(cmake/Custom.cmake)

# Set policies
#set_policy(CMP0054 NEW) # ENABLE CMP0054: Only interpret if() arguments as variables or keywords when unquoted.
#set_policy(CMP0042 NEW) # ENABLE CMP0042: MACOSX_RPATH is enabled by default.
#set_policy(CMP0063 NEW) # ENABLE CMP0063: Honor visibility properties for all target types.
#set_policy(CMP0077 NEW) # ENABLE CMP0077: option() honors normal variables

# Include cmake modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(GenerateExportHeader)

set(WriterCompilerDetectionHeaderFound NOTFOUND)
# This module is only available with CMake >=3.1, so check whether it could be found
# BUT in CMake 3.1 this module doesn't recognize AppleClang as compiler, so just use it as of CMake 3.2
if (${CMAKE_VERSION} VERSION_GREATER "3.2")
    include(WriteCompilerDetectionHeader OPTIONAL RESULT_VARIABLE WriterCompilerDetectionHeaderFound)
endif()

# Include custom cmake modules
include(cmake/GenerateTemplateExportHeader.cmake)
include(cmake/GetGitRevisionDescription.cmake)


# 
# Project description and (meta) information
# 

# Get git revision
get_git_head_revision(GIT_REFSPEC GIT_SHA1)
string(SUBSTRING "${GIT_SHA1}" 0 12 GIT_REV)
if(NOT GIT_SHA1)
    set(GIT_REV "0")
endif()

# Meta information about the project
set(META_PROJECT_NAME        "mvisus")
set(META_PROJECT_DESCRIPTION "MVisus project embryo")
set(META_AUTHOR_ORGANIZATION "My Company")
set(META_AUTHOR_DOMAIN       "https://github.com/vstanchevici/mvisus")
set(META_AUTHOR_MAINTAINER   "vstanchevici@email.com")
set(META_VERSION_MAJOR       "1")
set(META_VERSION_MINOR       "0")
set(META_VERSION_PATCH       "0")
set(META_VERSION_REVISION    "${GIT_REV}")
set(META_VERSION             "${META_VERSION_MAJOR}.${META_VERSION_MINOR}.${META_VERSION_PATCH}")
set(META_NAME_VERSION        "${META_PROJECT_NAME} v${META_VERSION} (${META_VERSION_REVISION})")
set(META_CMAKE_INIT_SHA      "${GIT_REV}")

string(MAKE_C_IDENTIFIER ${META_PROJECT_NAME} META_PROJECT_ID)
string(TOUPPER ${META_PROJECT_ID} META_PROJECT_ID)

# 
# Project configuration options
# 

# Project options
#option(BUILD_SHARED_LIBS      "Build shared instead of static libraries."              ON)
#option(OPTION_SELF_CONTAINED  "Create a self-contained install with all dependencies." OFF)
#option(OPTION_BUILD_TESTS     "Build tests."                                           ON)
#option(OPTION_BUILD_DOCS      "Build documentation."                                   OFF)
#option(OPTION_BUILD_EXAMPLES  "Build examples."                                        OFF)
#option(OPTION_ENABLE_COVERAGE "Add coverage information."                              OFF)

# 
# Declare project
# 

# Generate folders for IDE targets (e.g., VisualStudio solutions)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(IDE_FOLDER "")

# Declare project
project(${META_PROJECT_NAME} LANGUAGES C CXX)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

# Create version file
file(WRITE "${PROJECT_BINARY_DIR}/VERSION" "${META_NAME_VERSION}")

# Generate version-header
#configure_file(version.h.in ${CMAKE_CURRENT_BINARY_DIR}/include/${META_PROJECT_NAME}/${META_PROJECT_NAME}-version.h @ONLY)
#configure_file(version.h.in ../include/${META_PROJECT_NAME}-version.h @ONLY)

# detect operating system and host processor
#message(STATUS "We are on a ${CMAKE_SYSTEM_NAME} system")
#message(STATUS "The host processor is ${CMAKE_HOST_SYSTEM_PROCESSOR}")

#add_subdirectory(src)
add_executable(${PROJECT_NAME} src/main.cpp src/Core/App.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE src)

# 
# Deployment (global project files)
# 

# Install version file
#set(INSTALL_ROOT      "./test/")       # /usr/[local]/share/<project>
#install(FILES "${PROJECT_BINARY_DIR}/VERSION" DESTINATION ${INSTALL_ROOT} COMPONENT runtime)