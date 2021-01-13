#Armadillo

ExternalProject_Add(${armadillo}
    PREFIX                  ${PROJECT_BINARY_DIR}/third_party/${armadillo}
    GIT_REPOSITORY          https://gitlab.com/conradsnicta/armadillo-code.git
    GIT_TAG                 10.2.x
    INSTALL_DIR             install
    CMAKE_ARGS              -Dopenblas_LIBRARY:PATH=${OPENBLAS_INCLUDE_DIR} -DCMAKE_INSTALL_PREFIX:PATH=${PROJECT_BINARY_DIR}/third_party/${armadillo}/install
    UPDATE_DISCONNECTED     True
    UPDATE_COMMAND          ""
    #CONFIGURE_COMMAND      ""
    #BUILD_COMMAND          ""
    #INSTALL_COMMAND        ""
    #TEST_COMMAND           ""
)

set(ARMADILLO_INCLUDE_DIR ${PROJECT_BINARY_DIR}/third_party/${armadillo}/install/include PARENT_SCOPE)
set(ARMADILLO_LIB_DIR ${PROJECT_BINARY_DIR}/third_party/${armadillo}/install/lib PARENT_SCOPE)
set(ARMADILLO_LIB_NAME ${armadillo} PARENT_SCOPE)

