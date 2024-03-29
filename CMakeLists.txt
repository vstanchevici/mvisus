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

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    if (MVISUS_MSVC_STATIC_RUNTIME)
        set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    else()
        set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
    endif()
endif()

if(MSVC)
    target_compile_definitions(${PROJECT_NAME} PRIVATE -D_WIN32_WINNT=0x600 -D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS -D_WINSOCK_DEPRECATED_NO_WARNINGS)
endif()
if (MINGW)
    target_compile_definitions(${PROJECT_NAME} PRIVATE -D_WIN32_WINNT=0x600)
endif()


find_package(Armadillo REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${ARMADILLO_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${ARMADILLO_LIBRARIES})
#message(STATUS "ARMADILLO_INCLUDE_DIRS: " ${ARMADILLO_INCLUDE_DIRS})
#message(STATUS "ARMADILLO_LIBRARIES: " ${ARMADILLO_LIBRARIES})

#https://cmake.org/cmake/help/latest/module/FindBoost.html
find_package(Boost REQUIRED COMPONENTS serialization unit_test_framework)
target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${Boost_LIBRARIES})
#message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")

#https://stackoverflow.com/questions/59729745/cmake-mlpack-ubuntu-issue
find_package(mlpack REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${MLPACK_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${MLPACK_LIBRARIES})
#message(STATUS "MLPACK_INCLUDE_DIRS: " ${MLPACK_INCLUDE_DIRS})
#message(STATUS "MLPACK_LIBRARY: " ${MLPACK_LIBRARIES})

find_package(glm REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)

if(ARCL_PATH)
    add_subdirectory(${ARCL_PATH} ${CMAKE_BINARY_DIR}/arcl)
    set_property(DIRECTORY ${ARCL_PATH} APPEND PROPERTY COMPILE_DEFINITIONS "-DBUILD_TESTS=FALSE")
else()
    message(FATAL_ERROR "ARCL_PATH is not set!")
endif()

if(IMGUI_PATH)
    set(IMGUI_SRC 
            ${IMGUI_PATH}/imconfig.h
            ${IMGUI_PATH}/imgui.cpp
            ${IMGUI_PATH}/imgui.h
            ${IMGUI_PATH}/imgui_demo.cpp
            ${IMGUI_PATH}/imgui_draw.cpp
            ${IMGUI_PATH}/imgui_internal.h
            ${IMGUI_PATH}/imgui_tables.cpp
            ${IMGUI_PATH}/imgui_widgets.cpp
            ${IMGUI_PATH}/imstb_rectpack.h
            ${IMGUI_PATH}/imstb_textedit.h
            ${IMGUI_PATH}/imstb_truetype.h
    )
    
    # The naming scheme: mymap_<val>
    set(mymap_val1 key1) # Maps val1 => key1
    set(mymap_val2 key2) # Maps val2 => key2
    # ...
    set(avar val1) # Some key
    #message("Key ${avar} is mapped into: ${mymap_${avar}}")

    set(IMGUI_SRC ${IMGUI_SRC}
            ${IMGUI_PATH}/backends/imgui_impl_dx12.h
            ${IMGUI_PATH}/backends/imgui_impl_dx12.cpp
    )

    set(IMGUI_SRC ${IMGUI_SRC}
            ${IMGUI_PATH}/backends/imgui_impl_win32.h
            ${IMGUI_PATH}/backends/imgui_impl_win32.cpp
    )

    foreach(_source IN ITEMS ${IMGUI_SRC})
        get_filename_component(_source_path "${_source}" PATH)
        file(RELATIVE_PATH _source_path_rel "${IMGUI_PATH}" "${_source_path}")
        string(REPLACE "/" "\\" _group_path "${_source_path_rel}")
        source_group("source\\imgui\\${_group_path}" FILES "${_source}")
    endforeach()

    target_sources(${PROJECT_NAME} PRIVATE ${IMGUI_SRC})
    target_include_directories(${PROJECT_NAME} PRIVATE ${IMGUI_PATH})
else()
    message(FATAL_ERROR "IMGUI_PATH is not set!")
endif()


find_package(nlohmann_json REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

find_package(argparse REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC argparse::argparse)