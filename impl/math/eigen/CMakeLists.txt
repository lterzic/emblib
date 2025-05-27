# Use Eigen from this project's submodule
# Might cause conflict if there is another Eigen
# instance used elsewhere where emblib is used
add_submodule("external/eigen")

# Define the emblib-math-impl definition
add_library(emblib-math-impl INTERFACE)

# Link the Eigen library
target_link_libraries(emblib-math-impl
INTERFACE
    eigen
)

# Compile definitions to reduce binary size and
# prevent heap usage
target_compile_definitions(emblib-math-impl
INTERFACE
    EIGEN_NO_MALLOC
    EIGEN_NO_IO
)