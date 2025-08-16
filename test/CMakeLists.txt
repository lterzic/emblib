message(STATUS "Setting up tests")

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