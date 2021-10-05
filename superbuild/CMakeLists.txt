#
# === CMake lists for the mvisus ===
#


# === CMake options ===

# Set CMake version
cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

#[[
#https://discourse.cmake.org/t/how-to-set-cmake-msvc-runtime-library-for-an-external-project/2805/4
#set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
#set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
#set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

#https://github.com/oneapi-src/oneTBB/blob/master/CMakeLists.txt
if (POLICY CMP0091)
    # The NEW behavior for this policy is to not place MSVC runtime library flags in the default
    # CMAKE_<LANG>_FLAGS_<CONFIG> cache entries and use CMAKE_MSVC_RUNTIME_LIBRARY abstraction instead.
    cmake_policy(SET CMP0091 NEW)
    message(STATUS "CMAKE_MSVC_RUNTIME_LIBRARY -> ${CMAKE_MSVC_RUNTIME_LIBRARY}")
elseif (DEFINED CMAKE_MSVC_RUNTIME_LIBRARY)
    message(FATAL_ERROR "CMAKE_MSVC_RUNTIME_LIBRARY was defined while policy CMP0091 is not available. Use CMake 3.15 or newer.")
endif()
]]

#
# Configure CMake environment
#

# Declare project
project(mvisus-superbuild LANGUAGES C CXX)


# Set standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

include(ProcessorCount)
ProcessorCount(CPU_COUNT)
if(CPU_COUNT EQUAL 0)
    set(CPU_COUNT 1)
endif()
math(EXPR CPU_COUNT "${CPU_COUNT} / 2 + 1")


include(ExternalProject)

if(NOT DEFINED LIBRARIES_PREFIX)
    set(LIBRARIES_PREFIX ${CMAKE_BINARY_DIR})
endif()

if(NOT DEFINED LIBRARIES_INSTALL_PREFIX)
    set(LIBRARIES_INSTALL_PREFIX ${LIBRARIES_PREFIX}/installed/${CMAKE_SYSTEM_NAME})
endif()

if (WIN32)
    set(SHELL_EXTENSION ".bat")
    set(EXE_EXTENSION ".exe")
elseif (UNIX)
    set(SHELL_EXTENSION ".sh")
    set(EXE_EXTENSION "")
endif()


ExternalProject_Add(boost
    PREFIX                  ${LIBRARIES_PREFIX}/boost
    GIT_REPOSITORY          https://github.com/boostorg/boost.git
    GIT_TAG                 boost-1.74.0
    GIT_SHALLOW             1
    GIT_SUBMODULES_RECURSE  1
    BUILD_IN_SOURCE         ON
    CONFIGURE_COMMAND       ./bootstrap${SHELL_EXTENSION}
    BUILD_COMMAND           ./b2${EXE_EXTENSION} link=static variant=$<IF:$<CONFIG:Debug>,debug,release> threading=multi runtime-link=shared address-model=64 --without-python -j ${CPU_COUNT} install --prefix=${LIBRARIES_INSTALL_PREFIX}/boost
    
    UPDATE_DISCONNECTED     TRUE
    UPDATE_COMMAND          ""
    BUILD_ALWAYS            FALSE
    INSTALL_COMMAND         ""
)


ExternalProject_Add(armadillo
    PREFIX                  ${LIBRARIES_PREFIX}/armadillo
    GIT_REPOSITORY          https://gitlab.com/conradsnicta/armadillo-code.git
    GIT_TAG                 10.7.x
    CMAKE_ARGS              -DCMAKE_INSTALL_PREFIX:PATH=${LIBRARIES_INSTALL_PREFIX}/armadillo #-DCMAKE_DEBUG_POSTFIX=$<$<CONFIG:Debug>:d>  they dont support this now
    UPDATE_DISCONNECTED     TRUE
    UPDATE_COMMAND          ""
    BUILD_ALWAYS            FALSE
    #CONFIGURE_COMMAND      ""
    #BUILD_COMMAND          ""
    #INSTALL_COMMAND        ""
    #TEST_COMMAND           ""
)


ExternalProject_Add(cereal
    PREFIX                  ${LIBRARIES_PREFIX}/cereal
    DEPENDS                 boost
    GIT_REPOSITORY          https://github.com/USCiLab/cereal.git
    GIT_TAG                 v1.3.0
    CMAKE_ARGS              -DCMAKE_DEBUG_POSTFIX=$<$<CONFIG:Debug>:d> -DCMAKE_INSTALL_PREFIX:PATH=${LIBRARIES_INSTALL_PREFIX}/cereal -DCMAKE_PREFIX_PATH:PATH=${LIBRARIES_INSTALL_PREFIX}/boost -DJUST_INSTALL_CEREAL:BOOL=TRUE
    UPDATE_DISCONNECTED     TRUE
    UPDATE_COMMAND          ""
    BUILD_ALWAYS            FALSE
)

#https://www.statwks.com/c/
ExternalProject_Add(mlpack
    PREFIX                  ${LIBRARIES_PREFIX}/mlpack
    DEPENDS                 cereal armadillo
    GIT_REPOSITORY          https://github.com/mlpack/mlpack.git
    GIT_TAG                 3.4.2
    CMAKE_ARGS              -DCMAKE_INSTALL_PREFIX:PATH=${LIBRARIES_INSTALL_PREFIX}/mlpack -DCMAKE_DEBUG_POSTFIX=$<$<CONFIG:Debug>:d>

    CMAKE_CACHE_ARGS        -DBUILD_TESTS:BOOL=OFF
                            -DBUILD_SHARED_LIBS:BOOL=OFF
                            -DDOWNLOAD_ENSMALLEN:BOOL=ON
                            -DDOWNLOAD_STB_IMAGE:BOOL=ON
                            -DBUILD_R_BINDINGS:BOOL=OFF
                            -DBUILD_CLI_EXECUTABLES:BOOL=OFF
                            -DBUILD_GO_BINDINGS:BOOL=OFF
                            -DCMAKE_PREFIX_PATH:PATH=${LIBRARIES_INSTALL_PREFIX}/armadillo
                            -DBOOST_ROOT:PATH=${LIBRARIES_INSTALL_PREFIX}/boost
                            -DBoost_DEBUG:BOOL=ON
                            -DBoost_USE_STATIC_RUNTIME:BOOL=OFF
                            -DBoost_LIB_PREFIX:STRING=lib
                            #-DBoost_NO_BOOST_CMAKE:BOOL=FALSE

    UPDATE_DISCONNECTED     TRUE
    UPDATE_COMMAND          ""
    BUILD_ALWAYS            FALSE
)
#set_property(TARGET mlpack PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>") - not working

ExternalProject_Add(glfw
    PREFIX                  ${LIBRARIES_PREFIX}/glfw
    GIT_REPOSITORY          https://github.com/glfw/glfw.git
    GIT_TAG                 3.3.4
    CMAKE_ARGS              -DCMAKE_INSTALL_PREFIX:PATH=${LIBRARIES_INSTALL_PREFIX}/glfw
    CMAKE_CACHE_ARGS        -DGLFW_BUILD_DOCS:BOOL=OFF
                            -DGLFW_BUILD_TESTS:BOOL=OFF
                            -DGLFW_BUILD_EXAMPLES:BOOL=OFF
                            
    UPDATE_DISCONNECTED     TRUE
    UPDATE_COMMAND          ""
    BUILD_ALWAYS            FALSE
)

ExternalProject_Add(mvisus
    PREFIX              ${LIBRARIES_PREFIX}/mvisus
    DEPENDS             mlpack glfw
    SOURCE_DIR          ${PROJECT_SOURCE_DIR}/../
    CMAKE_ARGS          -DBoost_USE_STATIC_RUNTIME:BOOL=OFF #-DLIBRARIES_INSTALL_PREFIX=${LIBRARIES_INSTALL_PREFIX} -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
    CMAKE_CACHE_ARGS    -DCMAKE_PREFIX_PATH:STRING="${LIBRARIES_INSTALL_PREFIX}/armadillo;${LIBRARIES_INSTALL_PREFIX}/mlpack;" -DBOOST_ROOT:PATH=${LIBRARIES_INSTALL_PREFIX}/boost -DGLFW3_ROOT:PATH=${LIBRARIES_INSTALL_PREFIX}/glfw
    BUILD_ALWAYS        TRUE
    INSTALL_COMMAND     ""
)
