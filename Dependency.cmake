# ------ Set for ExternalProject ---------------- #
MESSAGE( STATUS "Setting for ExternalProject")

INCLUDE( ExternalProject )

MESSAGE( STATUS "Setting Done")
# ----------------------------------------------- #



# ------ Set Variables for Dependency ----------- #
MESSAGE( STATUS "Setting Variables for Dependency")

SET( DEP_INCLUDE )
SET( DEP_LIBS )
SET( DEP_LIST )

MESSAGE( STATUS "Setting Done")
# ----------------------------------------------- #



# ------ Set Options for Target Files ----------- #
MESSAGE( STATUS "Add Executable by platform")

## Include Header Directories for Target Files
IF ( WIN32 )
    SET( STATIC_LIB lib )
    SET( DYNAMIC_LIB dll)

ELSE ()
    SET( STATIC_LIB a )
    SET( DYNAMIC_LIB so )

ENDIF()

MESSAGE( STATUS "Setting Done" )
# ----------------------------------------------- #



#### GLM ----------------------------------- #
MESSAGE(STATUS "GLM - Linking ...")

EXTERNALPROJECT_ADD(
    GLM
    GIT_REPOSITORY GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG "1.0.1"

    PREFIX ${PREFIX_DIR}/GLM

    UPDATE_COMMAND "" PATCH_COMMAND "" TEST_COMMAND "" INSTALL_COMMAND ""
    CMAKE_ARGS
        -DGLM_BUILD_LIBRARY=OFF
        -DGLM_BUILD_TESTS=OFF
        -DGLM_IS_MASTER_PROJECT=OFF
        -DGLM_BUILD_INSTALL=OFF
        -DGLM_ENABLE_CXX_11=ON
        -DGLM_ENABLE_CXX_14=ON
        -DGLM_ENABLE_CXX_17=ON
        _DGLM_ENABLE_CXX_20=ON
)

LIST( APPEND INCS ${PREFIX_DIR}/GLM/src/GLM )

MESSAGE(STATUS "GLM - Done")
#### GLM ----------------------------------- #