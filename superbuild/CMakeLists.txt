 #
# === CMake lists for the mvisus ===
#


# === CMake options ===

# Set CMake version
cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

# Set paths
if(NOT DEFINED LIBRARIES_PREFIX)
    set(LIBRARIES_PREFIX ${CMAKE_BINARY_DIR})
endif()

if(NOT DEFINED LIBRARIES_INSTALL_PREFIX)
    set(LIBRARIES_INSTALL_PREFIX ${LIBRARIES_PREFIX}/install/${CMAKE_SYSTEM_NAME})
endif()

# If is not set CMAKE_INSTALL_PREFIX set it
if(NOT CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX ${LIBRARIES_INSTALL_PREFIX} CACHE STRING "Install location for mvisus." FORCE)
endif()

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

option(MVISUS_MSVC_STATIC_RUNTIME "Link with static msvc runtime libraries" ON)

message(STATUS "MVISUS_MSVC_STATIC_RUNTIME: ${MVISUS_MSVC_STATIC_RUNTIME}")

if (WIN32)
    set(SHELL_EXTENSION ".bat")
    set(EXE_EXTENSION ".exe")
elseif (UNIX)
    set(SHELL_EXTENSION ".sh")
    set(EXE_EXTENSION "")
endif()

include(ExternalProject)

#[[
ExternalProject_Add(boost
    PREFIX                  ${LIBRARIES_PREFIX}/boost
    GIT_REPOSITORY          https://github.com/boostorg/boost.git
    GIT_TAG                 boost-1.78.0
    GIT_SHALLOW             1
    GIT_SUBMODULES_RECURSE  1
    BUILD_IN_SOURCE         ON
    CONFIGURE_COMMAND       ./bootstrap${SHELL_EXTENSION}
    BUILD_COMMAND           ./b2${EXE_EXTENSION} link=static variant=$<IF:$<CONFIG:Debug>,debug,release> threading=multi runtime-link=$<IF:$<BOOL:${MVISUS_MSVC_STATIC_RUNTIME}>,static,shared> address-model=64 --without-python -j ${CPU_COUNT} install --prefix=${LIBRARIES_INSTALL_PREFIX}/boost
    
    UPDATE_DISCONNECTED     TRUE
    UPDATE_COMMAND          ""
    BUILD_ALWAYS            FALSE
    INSTALL_COMMAND         ""
)
]]

ExternalProject_Add(boost
    PREFIX                  ${LIBRARIES_PREFIX}/boost
    GIT_REPOSITORY          https://github.com/boostorg/boost.git
    GIT_TAG                 boost-1.78.0
    GIT_SHALLOW             1
    GIT_SUBMODULES_RECURSE  1
    CMAKE_ARGS              -DCMAKE_INSTALL_PREFIX:PATH=${LIBRARIES_INSTALL_PREFIX}/boost
    CMAKE_CACHE_ARGS        -DBOOST_RUNTIME_LINK:STRING=$<IF:$<BOOL:${MVISUS_MSVC_STATIC_RUNTIME}>,static,shared>
                            -DBUILD_SHARED_LIBS:BOOL=OFF

    UPDATE_DISCONNECTED     TRUE
    UPDATE_COMMAND          ""
    BUILD_ALWAYS            FALSE
)


ExternalProject_Add(armadillo
    PREFIX                  ${LIBRARIES_PREFIX}/armadillo
    GIT_REPOSITORY          https://gitlab.com/conradsnicta/armadillo-code.git
    GIT_TAG                 10.8.x
    CMAKE_ARGS              -DCMAKE_INSTALL_PREFIX:PATH=${LIBRARIES_INSTALL_PREFIX}/armadillo #-DCMAKE_DEBUG_POSTFIX=$<$<CONFIG:Debug>:d>  they dont support this now
    UPDATE_DISCONNECTED     TRUE
    UPDATE_COMMAND          ""
    BUILD_ALWAYS            FALSE
    #CONFIGURE_COMMAND      ""
    #BUILD_COMMAND          ""
    #INSTALL_COMMAND        ""
    #TEST_COMMAND           ""
)

ExternalProject_Add(stb
    PREFIX                  ${LIBRARIES_PREFIX}/stb
    GIT_REPOSITORY          https://github.com/nothings/stb.git
    UPDATE_DISCONNECTED     TRUE
    UPDATE_COMMAND          ""
    CONFIGURE_COMMAND       ""
    BUILD_COMMAND           ""
    INSTALL_COMMAND         ""
)
ExternalProject_Get_property(stb SOURCE_DIR)
set(STB_SOURCE_DIR ${SOURCE_DIR})

#5>-- Could NOT find Ensmallen (missing: ENSMALLEN_INCLUDE_DIR) (Required is at least version "2.10.0")
#5>-- Could NOT find StbImage (missing: STB_IMAGE_INCLUDE_DIR)
#https://www.statwks.com/c/
ExternalProject_Add(mlpack
    PREFIX                  ${LIBRARIES_PREFIX}/mlpack
    DEPENDS                 armadillo boost stb
    GIT_REPOSITORY          https://github.com/mlpack/mlpack.git
    GIT_TAG                 3.4.2
    CMAKE_ARGS              -DCMAKE_INSTALL_PREFIX:PATH=${LIBRARIES_INSTALL_PREFIX}/mlpack -DCMAKE_DEBUG_POSTFIX=$<$<CONFIG:Debug>:d>

    CMAKE_CACHE_ARGS        -DBUILD_TESTS:BOOL=OFF
                            -DBUILD_SHARED_LIBS:BOOL=OFF
                            -DDOWNLOAD_ENSMALLEN:BOOL=ON
                            -DDOWNLOAD_STB_IMAGE:BOOL=OFF
                            -DBUILD_R_BINDINGS:BOOL=OFF
                            -DBUILD_CLI_EXECUTABLES:BOOL=OFF
                            -DBUILD_GO_BINDINGS:BOOL=OFF
                            -DBUILD_JULIA_BINDINGS:BOOL=OFF
                            -DCMAKE_PREFIX_PATH:PATH=${LIBRARIES_INSTALL_PREFIX}/armadillo
                            -DSTB_IMAGE_FOUND:BOOL=TRUE
                            -DSTB_IMAGE_INCLUDE_DIR:STRING=${STB_SOURCE_DIR}
                            -DBOOST_ROOT:PATH=${LIBRARIES_INSTALL_PREFIX}/boost
                            -DBoost_USE_STATIC_RUNTIME:BOOL=${MVISUS_MSVC_STATIC_RUNTIME}
                            -DBoost_USE_STATIC_LIBS:BOOL=ON
                            -DBoost_LIB_PREFIX:STRING=lib
                            -DBoost_NO_BOOST_CMAKE:BOOL=FALSE
                            -DBoost_ADDITIONAL_VERSIONS:STRING="1.78"
                            #-DBoost_DEBUG:BOOL=ON
                            -DMLPACK_MSVC_STATIC_RUNTIME:BOOL=${MVISUS_MSVC_STATIC_RUNTIME}

    UPDATE_DISCONNECTED     TRUE
    UPDATE_COMMAND          ""
    BUILD_ALWAYS            FALSE
)
ExternalProject_Add_Step(mlpack replace_cmakelists
    DEPENDEES download
    DEPENDERS configure
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/patch/mlpack.3.4.2/CMakeLists.txt  ${LIBRARIES_PREFIX}/mlpack/src/mlpack/CMakeLists.txt
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/patch/mlpack.3.4.2/src/mlpack/CMakeLists.txt  ${LIBRARIES_PREFIX}/mlpack/src/mlpack/src/mlpack/CMakeLists.txt
)

ExternalProject_Add(glfw
    PREFIX                  ${LIBRARIES_PREFIX}/glfw
    GIT_REPOSITORY          https://github.com/glfw/glfw.git
    GIT_TAG                 3.3.4
    CMAKE_ARGS              -DCMAKE_INSTALL_PREFIX:PATH=${LIBRARIES_INSTALL_PREFIX}/glfw
    CMAKE_CACHE_ARGS        -DGLFW_BUILD_DOCS:BOOL=OFF
                            -DGLFW_BUILD_TESTS:BOOL=OFF
                            -DGLFW_BUILD_EXAMPLES:BOOL=OFF
                            -DUSE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=$<IF:$<BOOL:${MVISUS_MSVC_STATIC_RUNTIME}>,OFF,ON>

    UPDATE_DISCONNECTED     TRUE
    UPDATE_COMMAND          ""
    BUILD_ALWAYS            FALSE
)

ExternalProject_Add(glm
    PREFIX                  ${LIBRARIES_PREFIX}/glm
    GIT_REPOSITORY          https://github.com/g-truc/glm.git
    GIT_TAG                 master
    CMAKE_ARGS              -DCMAKE_INSTALL_PREFIX:PATH=${LIBRARIES_INSTALL_PREFIX}/glm
    CMAKE_CACHE_ARGS        -DBUILD_TESTING:BOOL=OFF
                            -DBUILD_SHARED_LIBS:BOOL=OFF
                            -DBUILD_STATIC_LIBS:BOOL=ON

    UPDATE_DISCONNECTED     TRUE
    UPDATE_COMMAND          ""
    BUILD_ALWAYS            FALSE
)


ExternalProject_Add(imgui
    PREFIX                  ${LIBRARIES_PREFIX}/imgui
    DEPENDS                 glfw
    GIT_REPOSITORY          https://github.com/ocornut/imgui.git
    GIT_TAG                 v1.84.2
    UPDATE_DISCONNECTED     TRUE
    UPDATE_COMMAND          ""
    CONFIGURE_COMMAND       ""
    BUILD_COMMAND           ""
    INSTALL_COMMAND         ""
)
ExternalProject_Get_property(imgui SOURCE_DIR)
set(IMGUI_SOURCE_DIR ${SOURCE_DIR})

ExternalProject_Add(arcl
    PREFIX                  ${LIBRARIES_PREFIX}/arcl
    SOURCE_DIR              ${PROJECT_SOURCE_DIR}/../../arcl
    UPDATE_DISCONNECTED     TRUE
    UPDATE_COMMAND          ""
    CONFIGURE_COMMAND       ""
    BUILD_COMMAND           ""
    INSTALL_COMMAND         ""
)
ExternalProject_Get_property(arcl SOURCE_DIR)
set(ARCL_PATH ${SOURCE_DIR})

ExternalProject_Add(mvisus
    PREFIX              ${LIBRARIES_PREFIX}/mvisus
    DEPENDS             mlpack imgui glm arcl
    SOURCE_DIR          ${PROJECT_SOURCE_DIR}/../
    CMAKE_CACHE_ARGS    #-DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
                        -DMVISUS_MSVC_STATIC_RUNTIME:BOOL=${MVISUS_MSVC_STATIC_RUNTIME}
                        -DCMAKE_PREFIX_PATH:STRING= ${LIBRARIES_INSTALL_PREFIX}/armadillo
                                                    ${LIBRARIES_INSTALL_PREFIX}/mlpack
                                                    ${LIBRARIES_INSTALL_PREFIX}/glm
                        -DBOOST_ROOT:PATH=${LIBRARIES_INSTALL_PREFIX}/boost
                        -DBoost_USE_STATIC_RUNTIME:BOOL=${MVISUS_MSVC_STATIC_RUNTIME}
                        -DBoost_USE_STATIC_LIBS:BOOL=ON
                        -DBoost_ADDITIONAL_VERSIONS:STRING="1.78"
                        -DGLFW3_ROOT:PATH=${LIBRARIES_INSTALL_PREFIX}/glfw
                        -DIMGUI_PATH:PATH=${IMGUI_SOURCE_DIR}
                        -DSTB_SOURCE_DIR:PATH=${STB_SOURCE_DIR}
                        -DARCL_PATH:PATH=${ARCL_PATH}

    BUILD_ALWAYS        TRUE
    INSTALL_COMMAND     ""
)
