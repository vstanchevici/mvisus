#Boost

#this is for cmake, but now is not supported
#ExternalProject_Add(${Boost}
  #PREFIX            ${PROJECT_BINARY_DIR}/third_party/${LBoost}
  #GIT_REPOSITORY    https://github.com/boostorg/boost.git
  #GIT_TAG           boost-1.75.0
  #GIT_SUBMODULES_RECURSE 1
  #GIT_SHALLOW       1
  #INSTALL_DIR       install
  #CMAKE_ARGS        -DCMAKE_INSTALL_PREFIX:PATH=${PROJECT_BINARY_DIR}/third_party/${LBoost}/install
  #CMAKE_CACHE_ARGS  -DBOOST_ENABLE_CMAKE:BOOL=TRUE
  #CONFIGURE_COMMAND ""
  #BUILD_COMMAND     ""
  #INSTALL_COMMAND   ""
  #TEST_COMMAND      ""
#)

ExternalProject_Add(${boost}
    PREFIX                  ${PROJECT_BINARY_DIR}/third_party/${boost}
    GIT_REPOSITORY          https://github.com/boostorg/boost.git
    GIT_TAG                 boost-1.75.0
    GIT_SHALLOW             1
    GIT_SUBMODULES_RECURSE  1
    BUILD_IN_SOURCE         1
    CONFIGURE_COMMAND       ${PROJECT_BINARY_DIR}/third_party/${boost}/src/Boost/bootstrap.bat
    BUILD_COMMAND           ${PROJECT_BINARY_DIR}/third_party/${boost}/src/Boost/b2.exe install link=static variant=release,debug threading=multi runtime-link=static address-model=64 threading=multi --without-python --prefix=${PROJECT_BINARY_DIR}/third_party/${boost}/install
    INSTALL_DIR             install
    INSTALL_COMMAND         ""
    UPDATE_DISCONNECTED     True
    UPDATE_COMMAND          ""
)


set(BOOST_IMCLUDE_DIR ${PROJECT_BINARY_DIR}/third_party/${boost}/install/include/boost-1_75 PARENT_SCOPE)
set(BOOST_LIB_DIR ${PROJECT_BINARY_DIR}/third_party/${boost}/install/lib PARENT_SCOPE)
set(BOOST_LIB_NAME ${boost} PARENT_SCOPE)