# Use the default FreeRTOS configuration
add_library(freertos_config INTERFACE)
target_include_directories(freertos_config
SYSTEM INTERFACE
    rtos/freertos
)
target_compile_definitions(freertos_config
INTERFACE
    projCOVERAGE_TEST=0
    projENABLE_TRACING=0
)

# Using FreeRTOS and Eigen as default implementations
# TODO: Make test for each implementation
add_subdirectory(${PROJECT_SOURCE_DIR}/drivers/rtos/freertos ${CMAKE_CURRENT_BINARY_DIR}/rtos/freertos)
add_subdirectory(${PROJECT_SOURCE_DIR}/drivers/math/eigen ${CMAKE_CURRENT_BINARY_DIR}/math/eigen)

# Add the testing library and all the tests
add_submodule("${PROJECT_SOURCE_DIR}/lib/Catch2" ${PROJECT_BINARY_DIR}/lib/Catch2)

# Flags for the build
target_compile_options(emblib PUBLIC -Wall -Wextra -Wpedantic)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

enable_testing()

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
)

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain emblib)

include(CTest)
include(Catch)
catch_discover_tests(tests)

add_executable(test_main test.cpp)
target_link_libraries(test_main PRIVATE emblib)