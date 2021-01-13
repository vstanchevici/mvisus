#cereal

ExternalProject_Add(${cereal}
    PREFIX                  ${PROJECT_BINARY_DIR}/third_party/${cereal}
    GIT_REPOSITORY          https://github.com/USCiLab/cereal.git
    GIT_TAG                 master
    INSTALL_DIR             install
    CMAKE_ARGS              -DCMAKE_INSTALL_PREFIX:PATH=${PROJECT_BINARY_DIR}/third_party/${cereal}/install
    CMAKE_CACHE_ARGS        -DBoost_INCLUDE_DIR:PATH=${BOOST_IMCLUDE_DIR} -DBUILD_TESTS:BOOL=FALSE -DSKIP_PERFORMANCE_COMPARISON:BOOL=TRUE
    UPDATE_DISCONNECTED     True
    UPDATE_COMMAND          ""
)

set(CEREAL_INCLUDE_DIR ${PROJECT_BINARY_DIR}/third_party/${cereal}/install/include PARENT_SCOPE)
