# All tests are ran as a single executable currently
add_executable(tests
    dsp/kalman.test.cpp
    dsp/iir.test.cpp
    dsp/pid.test.cpp
    io/iodev.test.cpp
    math/matrix.test.cpp
    math/vector.test.cpp
    math/quaternion.test.cpp
    rtos/spinlock.test.cpp
    lockfree/allocator.test.cpp
    lockfree/spsc_queue.test.cpp
    lockfree/spmc_queue.test.cpp
)

# Flags for the build
target_compile_options(tests
PUBLIC
    -Wall
    -Wextra
    -Wpedantic
)

# Using Catch2 library for testing
target_link_libraries(tests
PRIVATE
    Catch2::Catch2WithMain
    emblib
    emblib_math_eigen
)

catch_discover_tests(tests)

# Dummy executable to test simple things
add_executable(test_main test.cpp)
target_link_libraries(test_main PRIVATE emblib)