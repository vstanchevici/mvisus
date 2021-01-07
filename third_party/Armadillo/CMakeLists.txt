#Armadillo

string(TOLOWER ${Armadillo} LArmadillo)

ExternalProject_Add(${Armadillo}
  PREFIX            ${PROJECT_BINARY_DIR}/third_party/${LArmadillo}
  GIT_REPOSITORY    https://gitlab.com/conradsnicta/armadillo-code.git
  GIT_TAG           10.2.x
  INSTALL_DIR       install
  CMAKE_ARGS        -Dopenblas_LIBRARY:PATH=${OpenBLASIncludeDir};-DCMAKE_INSTALL_PREFIX:PATH=${PROJECT_BINARY_DIR}/third_party/${LArmadillo}/install
  #CONFIGURE_COMMAND ""
  #BUILD_COMMAND     ""
  #INSTALL_COMMAND   ""
  #TEST_COMMAND      ""
)

set(ArmadilloIncludeDir ${PROJECT_BINARY_DIR}/third_party/${LArmadillo}/install/include PARENT_SCOPE)
set(ArmadilloLibDir ${PROJECT_BINARY_DIR}/third_party/${LArmadillo}/install/lib PARENT_SCOPE)
set(ArmadilloLibName ${LArmadillo} PARENT_SCOPE)

