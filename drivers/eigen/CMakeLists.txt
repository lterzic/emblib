FetchContent_Declare(
    eigen
    GIT_REPOSITORY  https://gitlab.com/libeigen/eigen.git
    GIT_TAG         5.0.1
    GIT_SHALLOW     TRUE
)
FetchContent_MakeAvailable(eigen)

add_library(emblib_eigen INTERFACE)

# Link the Eigen library
target_link_libraries(emblib_eigen
INTERFACE
    emblib
    eigen
)

# Compile definitions to reduce binary size and prevent heap usage
target_compile_definitions(emblib_eigen
INTERFACE
    EIGEN_NO_MALLOC
    EIGEN_NO_IO
    EIGEN_NO_DEBUG
    EIGEN_DONT_VECTORIZE
)

# Provide header implementations
target_include_prefix(emblib_eigen
INTERFACE
    details ${CMAKE_CURRENT_SOURCE_DIR}/backend
)