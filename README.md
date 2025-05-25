# emblib

C++ library for embedded development.

Intended to provide a consistent API to different hardware and software components, to enable writing platform-independent code. Provides wrappers for common dependencies such as math or RTOS libraries, Eigen and FreeRTOS implementations are provided. Standard libraries are used lightly, for headers like \<cstdint\> or \<functional\>, and dynamic allocation is completely avoided. Underlying library dependencies (such as FreeRTOS) can be configured in parent projects through CMake or auto configured with default settings by emblib.

Some APIs:
- Drivers
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
As emblib depends on other libraries which are fetched as git submodules, the easiest way to include all of them is to clone this repository recursively into the project.
```shell
git clone --recursive https://github.com/terzaterza/emblib.git
```
The library can then be included to a CMake project by adding this subdirectory, and linking the library to your target. CMake library (target) name is `emblib`.
```cmake
# Create the emblib_config target here if needed
# View the next section for info about the config

add_subdirectory("<path-to-cloned-repo>")
target_link_libraries(<target> PUBLIC/PRIVATE emblib)
```
The header files will then be available via the path from the [include](include/) folder. Headers follow the namespace hierarchy according to the file path, for example a class defined in the [include/emblib/math](include/emblib/math/) folder will be in the namespace `emblib::math`.

Header files are meant to be standalone, meaning it's enough to include only the file where the class you need is defined. All the necessary dependencies, including your custom emblib configuration, are included automatically.

To use a `matrix` class for example, you would add the following to your code:
```cpp
#include <emblib/math/matrix.hpp>

void your_function()
{
    emblib::math::matrix<float, 3, 3> my_matrix {/* elements */};
}
```

## CMake configuration

### emblib
Configuration header file can (should) be provided by creating `emblib_config` target, before adding the emblib subdirectory, with the include directory attached to the target providing the `emblib_config.hpp`.
```cmake
# EMBLIB configuration
add_library(emblib_config INTERFACE)

target_include_directories(emblib_config INTERFACE
    "<path-to-emblib_config.hpp>"
)
```
If this target is not provided, a default configuration is used.

### FreeRTOS
FreeRTOS port will be provided by default, depending on the host operating system (GCC_POSIX on Linux). This can be overriden by specifying the port by setting the `FREERTOS_PORT` in a parent CMake project.

For the configuration, `freertos_config` target can be provided with the include directory attached providing the `FreeRTOSConfig.h` file. If one does not exist, a default config file will be used, based on the GCC_POSIX port.

FreeRTOS (static) library can also be provided, be creating `freertos_kernel` target in a parent CMake project.

## Project structure
Source files are split between `src` and `include` folders, where files in the `src` folder are used only within this project, but the files in the `include` folder are meant to be public, ie. included by projects that use this library.

Dependencies (some of which might be optional) are located in the `external` folder, as git submodules.

Testing is done using the `Catch2` framework and all the related files are in the `test` folder.
