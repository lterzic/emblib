FetchContent_Declare(
    eigen
    GIT_REPOSITORY  https://gitlab.com/libeigen/eigen.git
    GIT_TAG         5.0.1
    GIT_SHALLOW     TRUE
)
FetchContent_MakeAvailable(eigen)

add_library(emblib_math_eigen INTERFACE)

# Link the Eigen library
target_link_libraries(emblib_math_eigen
INTERFACE
    emblib
    eigen
)

# Compile definitions to reduce binary size and
# prevent heap usage
target_compile_definitions(emblib_math_eigen
INTERFACE
    EIGEN_NO_MALLOC
    EIGEN_NO_IO
)

# Provide header implementations
# TODO: This should be PRIVATE somehow (not directly used)
target_include_directories(emblib_math_eigen
INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)