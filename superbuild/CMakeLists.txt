#
# === CMake lists for the mvisus ===
#


# === CMake options ===

# Set CMake version
cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

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

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    #https://discourse.cmake.org/t/how-to-set-cmake-msvc-runtime-library-for-an-external-project/2805/4
    #set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    #set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
    #set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

ExternalProject_Add(boost
    PREFIX                  ${LIBRARIES_PREFIX}/boost
    GIT_REPOSITORY          https://github.com/boostorg/boost.git
    GIT_TAG                 boost-1.74.0
    GIT_SHALLOW             1
    GIT_SUBMODULES_RECURSE  1
    BUILD_IN_SOURCE         ON
    CONFIGURE_COMMAND       ./bootstrap${SHELL_EXTENSION}
    BUILD_COMMAND           ./b2${EXE_EXTENSION} link=static variant=$<IF:$<CONFIG:Debug>,debug,release> threading=multi runtime-link=shared address-model=64 threading=multi --without-python -j ${CPU_COUNT} install --prefix=${LIBRARIES_INSTALL_PREFIX}/boost
    
    UPDATE_DISCONNECTED     TRUE
    UPDATE_COMMAND          ""
    BUILD_ALWAYS            FALSE
    INSTALL_COMMAND         ""
)


ExternalProject_Add(armadillo
    PREFIX                  ${LIBRARIES_PREFIX}/armadillo
    GIT_REPOSITORY          https://gitlab.com/conradsnicta/armadillo-code.git
    GIT_TAG                 10.7.x
    CMAKE_ARGS              -DCMAKE_DEBUG_POSTFIX=$<$<CONFIG:Debug>:d> -DCMAKE_INSTALL_PREFIX:PATH=${LIBRARIES_INSTALL_PREFIX}/armadillo
    CMAKE_CACHE_ARGS        -DCMAKE_MSVC_RUNTIME_LIBRARY=${CMAKE_MSVC_RUNTIME_LIBRARY} 
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
    CMAKE_CACHE_ARGS        -DCMAKE_MSVC_RUNTIME_LIBRARY=${CMAKE_MSVC_RUNTIME_LIBRARY} 
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
    CMAKE_ARGS              -DCMAKE_INSTALL_PREFIX:PATH=${LIBRARIES_INSTALL_PREFIX}/mlpack 
                            -DBUILD_TESTS:BOOL=OFF
                            -DBUILD_SHARED_LIBS:BOOL=OFF
                            -DDOWNLOAD_ENSMALLEN:BOOL=ON
                            -DDOWNLOAD_STB_IMAGE:BOOL=OFF
                            -DBUILD_R_BINDINGS:BOOl=OFF
                            -DBUILD_CLI_EXECUTABLES:BOOL=OFF
                            -DBUILD_GO_BINDINGS:BOOL=OFF
                            -DCMAKE_DEBUG_POSTFIX=$<$<CONFIG:Debug>:d>
                            -DCMAKE_PREFIX_PATH=${LIBRARIES_INSTALL_PREFIX}
                            -DARMADILLO_INCLUDE_DIR:PATH=${LIBRARIES_INSTALL_PREFIX}/armadillo/include
                            -DARMADILLO_LIBRARY:FILEPATH=${LIBRARIES_INSTALL_PREFIX}/armadillo/lib/armadillo.lib
                            -DBoost_INCLUDE_DIR:PATH=${LIBRARIES_INSTALL_PREFIX}/boost/include/boost-1_74
                            -DBoost_LIBRARY_DIR_DEBUG:PATH=${LIBRARIES_INSTALL_PREFIX}/boost/lib
                            -DBoost_LIBRARY_DIR_RELEASE:PATH=${LIBRARIES_INSTALL_PREFIX}/boost/lib
                            -DBoost_SERIALIZATION_LIBRARY_DEBUG:FILEPATH=${LIBRARIES_INSTALL_PREFIX}/boost/lib/libboost_serialization-vc142-mt-sgd-x64-1_74.lib
                            -DBoost_SERIALIZATION_LIBRARY_RELEASE:FILEPATH=${LIBRARIES_INSTALL_PREFIX}/lib/libboost_serialization-vc142-mt-sgd-x64-1_74.lib
                            -DBoost_UNIT_TEST_FRAMEWORK_LIBRARY_DEBUG:FILEPATH=${LIBRARIES_INSTALL_PREFIX}/boost/lib/libboost_unit_test_framework-vc142-mt-sgd-x64-1_74.lib
                            -DBoost_UNIT_TEST_FRAMEWORK_LIBRARY_RELEASE:FILEPATH=${LIBRARIES_INSTALL_PREFIX}/boost/lib/libboost_unit_test_framework-vc142-mt-sgd-x64-1_74.lib

    CMAKE_CACHE_ARGS        -DCMAKE_MSVC_RUNTIME_LIBRARY=${CMAKE_MSVC_RUNTIME_LIBRARY} 

    UPDATE_DISCONNECTED     TRUE
    UPDATE_COMMAND          ""
    BUILD_ALWAYS            FALSE
)


ExternalProject_Add(mvisus
    PREFIX              ${LIBRARIES_PREFIX}/mvisus
    DEPENDS             mlpack
    SOURCE_DIR          ${PROJECT_SOURCE_DIR}/../
    CMAKE_ARGS          -DLIBRARIES_INSTALL_PREFIX=${LIBRARIES_INSTALL_PREFIX} #-DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
    BUILD_ALWAYS        TRUE
    INSTALL_COMMAND     ""
)
