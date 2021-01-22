#
# === CMake lists for the mvisus ===
#


# === CMake options ===

# Set CMake version
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

# Remove ZERO_CHECK target
#set(CMAKE_SUPPRESS_REGENERATION true)

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
# === Project description and (meta) information ===
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
set(META_CMAKE_INIT_SHA      "${GIT_SHA1}")

string(MAKE_C_IDENTIFIER ${META_PROJECT_NAME} META_PROJECT_ID)
string(TOUPPER ${META_PROJECT_ID} META_PROJECT_ID)

#https://awesomeopensource.com/project/cginternals/cmake-init

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

# Set src and include root path
set(SRC_ROOT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/source/")
set(INCLUDE_ROOT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include/")

# Generate version-header
configure_file(${INCLUDE_ROOT_PATH}/version.h.in ${INCLUDE_ROOT_PATH}/${META_PROJECT_NAME}-version.h @ONLY)

file(
    GLOB_RECURSE INCLUDE_LIST
    LIST_DIRECTORIES false
    "${INCLUDE_ROOT_PATH}/*.hpp"
    "${INCLUDE_ROOT_PATH}/*.h"
)

file(
    GLOB_RECURSE SOURCE_LIST
    LIST_DIRECTORIES false
    "${SRC_ROOT_PATH}/*.c"
    "${SRC_ROOT_PATH}/*.cpp"
    "${SRC_ROOT_PATH}/*.h"
    "${SRC_ROOT_PATH}/*.hpp"
)

foreach(_include IN ITEMS ${INCLUDE_LIST})
    get_filename_component(_include_path "${_include}" PATH)
    file(RELATIVE_PATH _include_path_rel "${INCLUDE_ROOT_PATH}" "${_include_path}")
    string(REPLACE "/" "\\" _group_path2 "${_include_path_rel}")
    source_group("include\\${_group_path2}" FILES "${_include}")
endforeach()

foreach(_source IN ITEMS ${SOURCE_LIST})
    get_filename_component(_source_path "${_source}" PATH)
    file(RELATIVE_PATH _source_path_rel "${SRC_ROOT_PATH}" "${_source_path}")
    string(REPLACE "/" "\\" _group_path "${_source_path_rel}")
    source_group("source\\${_group_path}" FILES "${_source}")
endforeach()


add_executable(${PROJECT_NAME} ${INCLUDE_LIST} ${SOURCE_LIST})
target_include_directories(${PROJECT_NAME} PRIVATE  ${INCLUDE_ROOT_PATH} ${SRC_ROOT_PATH})



#
# Start adding libraries
#

#include cmake module
include(ExternalProject)

#Adding OpenBLAS library
#later must see how to integrate
#set(openblas "openblas")
#set(OPENBLAS_INCLUDE_DIR "")
#set(OPENBLAS_LIB_DIR "")
#set(OPENBLAS_LIB_NAME "")

#add_subdirectory(third_party/${openblas})
#add_dependencies(${PROJECT_NAME} ${openblas})
#target_include_directories(${PROJECT_NAME} PRIVATE ${OPENBLAS_INCLUDE_DIR})
#target_link_directories(${PROJECT_NAME} PRIVATE ${OPENBLAS_LIB_DIR})
#target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENBLAS_LIB_NAME})


#Adding Armadillo library
set(armadillo "armadillo")
set(ARMADILLO_INCLUDE_DIR "")
set(ARMADILLO_LIB_DIR "")
set(ARMADILLO_LIB_NAME "")
add_subdirectory(third_party/${armadillo})


#add_dependencies(${armadillo} ${openblas})
add_dependencies(${PROJECT_NAME} ${armadillo})
target_include_directories(${PROJECT_NAME} PRIVATE ${ARMADILLO_INCLUDE_DIR})
target_link_directories(${PROJECT_NAME} PRIVATE ${ARMADILLO_LIB_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${ARMADILLO_LIB_NAME})


#Adding Boost library
set(boost "boost")
set(BOOST_IMCLUDE_DIR "")
set(BOOST_LIB_DIR "")
set(BOOST_LIB_NAME "")
add_subdirectory(third_party/${boost})

target_include_directories(${PROJECT_NAME} PRIVATE ${BOOST_IMCLUDE_DIR})
target_link_directories(${PROJECT_NAME} PRIVATE ${BOOST_LIB_DIR})
#target_link_libraries(${PROJECT_NAME} PRIVATE ${BOOST_LIB_NAME})


#Adding cereal library
set(cereal "cereal")
set(CEREAL_INCLUDE_DIR "")
add_subdirectory(third_party/${cereal})

add_dependencies(${cereal} ${boost})
target_include_directories(${PROJECT_NAME} PRIVATE ${CEREAL_INCLUDE_DIR})


#Adding mlpack library
set(mlpack "mlpack")
set(MLPACK_INCLUDE_DIR "")
set(MLPACK_LIB_DIR "")
set(MLPACK_LIB_NAME "")
add_subdirectory(third_party/${mlpack})

add_dependencies(${mlpack} ${cereal})
add_dependencies(${mlpack} ${armadillo})
add_dependencies(${PROJECT_NAME} ${mlpack})
target_include_directories(${PROJECT_NAME} PRIVATE ${MLPACK_INCLUDE_DIR})
target_link_directories(${PROJECT_NAME} PRIVATE ${MLPACK_LIB_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${MLPACK_LIB_NAME})






#https://stackoverflow.com/questions/3016448/how-can-i-get-cmake-to-find-my-alternative-boost-installation
#https://stackoverflow.com/questions/6352123/multiple-directories-under-cmake
#https://stackoverflow.com/questions/51675100/how-can-i-prevent-a-cmake-external-project-from-updating-reinstalling-whenever

#set(var2 "var2" CACHE INTERNAL "")

# 
# Deployment (global project files)
# 

# Install version file
#message(STATUS "some message ...")
#set(INSTALL_ROOT      "./test/")       # /usr/[local]/share/<project>
#install(FILES "${PROJECT_BINARY_DIR}/VERSION" DESTINATION ${INSTALL_ROOT} COMPONENT runtime)
