# emblib

C++ library for embedded development.

Intended to provide a consistent API to different hardware and software components, to enable writing platform-independent code. Provides wrappers for common dependencies such as math or RTOS libraries, Eigen and FreeRTOS implementations are provided. Standard libraries are used lightly, for headers like \<cstdint\> or \<cstddef\>. Underlying library dependencies (such as FreeRTOS) can be configured in parent projects through CMake or auto configured with default settings by emblib.

Some APIs:
- Devices
    - Serial (char) devices - I2C, SPI
    - Sensors - Accelerometer, Gyro
    - GPIO
- RTOS
    - Mutex
    - Task (Thread)
    - Queue
- Math
    - Matrix
    - Vector
    - Quaternion
- DSP
    - Kalman filter (EKF)
    - IIR filter
    - PID controller

## Adding emblib to a project
It's enough to clone (or add as a submodule) emblib into your project. Although emblib depends on other libraries (submodules), they are fetched during CMake configuration only if there are actually required based on which drivers are used.

```shell
git clone https://github.com/lterzic/emblib.git
```

The library can then be included to a CMake project by adding this subdirectory, and linking to the `emblib` target. `emblib` is an INTERFACE target meaning it only provides headers with API (interface) definitions. Implementations of these interfaces can be found in the `drivers` folder. Each of these drivers is a separate target which can be linked to your project by first adding the subdirectory of the driver, and then linking to the driver target.

```cmake
# Assuming emblib can be found at directory libs/emblib,
# the following adds the core interface and Eigen implementation
# to the "app" executable

add_subdirectory("libs/emblib")
add_subdirectory("libs/emblib/drivers/math/eigen")

target_link_libraries(app PRIVATE emblib emblib_math_eigen)
```

Namespace hierarchy follows the file path hierarchy, starting from the [include](include/) folder, for example a class defined in the [include/emblib/math](include/emblib/math/) folder will be in the namespace `emblib::math`. Header files are meant to be standalone, meaning it's enough to include only the file where the class you need is defined. All the necessary dependencies, including your custom emblib configuration, are included automatically.

To use a `matrix` class (which uses underlying Eigen matrices), you would add the following to your code:
```cpp
#include <emblib/math/matrix.hpp>

void your_function()
{
    emblib::math::matrix<float, 3, 3> my_matrix {/* elements */};
}
```

## Testing

Testing is currently done using Catch2. All test cases are bundled into a single executable defined in the
[test/CMakeLists.txt](test/CMakeLists.txt) called tests. It is automatically built if this is the top level CMake project, but can also be manually built by adding the [test](test/) subdirectory from a parent CMake.

Building generates the executable `build/test/tests` which runs all the test cases.