#OpenBLAS

ExternalProject_Add(${openblas}
    PREFIX                  ${PROJECT_BINARY_DIR}/third_party/${openblas}
    GIT_REPOSITORY          https://github.com/xianyi/OpenBLAS.git
    GIT_TAG                 v0.3.13
    INSTALL_DIR             install
    CMAKE_ARGS              -DCMAKE_INSTALL_PREFIX:PATH=${PROJECT_BINARY_DIR}/third_party/${openblas}/install
    CMAKE_CACHE_ARGS        -DBUILD_WITHOUT_CBLAS:BOOL=FALSE -DBUILD_WITHOUT_LAPACK:BOOL=FALSE -DBUILD_RELAPACK:BOOL=FALSE -DDYNAMIC_ARCH:BOOL=FALSE
    UPDATE_DISCONNECTED     True
    UPDATE_COMMAND          ""
)

set(OPENBLAS_INCLUDE_DIR ${PROJECT_BINARY_DIR}/third_party/${openblas}/install/include PARENT_SCOPE)
set(OPENBLAS_LIB_DIR ${PROJECT_BINARY_DIR}/third_party/${openblas}/install/lib PARENT_SCOPE)
set(OPENBLAS_LIB_NAME ${openblas} PARENT_SCOPE)

