#OpenBLAS

string(TOLOWER ${OpenBLAS} LOpenBLAS)

ExternalProject_Add(${OpenBLAS}
  PREFIX            ${PROJECT_BINARY_DIR}/third_party/${LOpenBLAS}
  GIT_REPOSITORY    https://github.com/xianyi/OpenBLAS.git
  GIT_TAG           v0.3.13
  INSTALL_DIR       install
  CMAKE_ARGS        -DCMAKE_INSTALL_PREFIX:PATH=${PROJECT_BINARY_DIR}/third_party/${LOpenBLAS}/install
  #CONFIGURE_COMMAND ""
  #BUILD_COMMAND     ""
  #INSTALL_COMMAND   ""
  #TEST_COMMAND      ""
)

set(OpenBLASIncludeDir ${PROJECT_BINARY_DIR}/third_party/${LOpenBLAS}/install/include PARENT_SCOPE)
set(OpenBLASLibDir ${PROJECT_BINARY_DIR}/third_party/${LOpenBLAS}/install/lib PARENT_SCOPE)
set(OpenBLASLibName ${LOpenBLAS} PARENT_SCOPE)

