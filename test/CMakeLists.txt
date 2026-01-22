# Add the testing library and all the tests
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY  https://github.com/catchorg/Catch2.git
    GIT_TAG         v3.11.0
    GIT_SHALLOW     TRUE
)
FetchContent_MakeAvailable(Catch2)

# All tests are ran as a single executable currently
add_executable(tests
    dsp/kalman.test.cpp
    dsp/iir.test.cpp
    dsp/pid.test.cpp
    io/iostream.test.cpp
    math/matrix.test.cpp
    math/vector.test.cpp
    math/quaternion.test.cpp
    rtos/freertos/hooks.c
    rtos/queue.test.cpp
    rtos/mutex.test.cpp
    rtos/spinlock.test.cpp
    lockfree/allocator.test.cpp
    lockfree/spsc_queue.test.cpp
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
    emblib_rtos_freertos
)

# Use the default FreeRTOS configuration
target_include_directories(freertos_config
SYSTEM INTERFACE
    rtos/freertos
)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

include(CTest)
include(Catch)
catch_discover_tests(tests)

# Dummy executable to test simple things
add_executable(test_main test.cpp)
target_link_libraries(test_main PRIVATE emblib)