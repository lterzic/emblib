# Include directory containing the FreeRTOSConfig.h file should
# be added to this target by the user.
add_library(freertos_config INTERFACE)
target_compile_definitions(freertos_config
INTERFACE
    projCOVERAGE_TEST=0
    projENABLE_TRACING=0
)

FetchContent_Declare(
    freertos_kernel
    GIT_REPOSITORY  https://github.com/FreeRTOS/FreeRTOS-Kernel.git
    GIT_TAG         V11.2.0
    GIT_SHALLOW     TRUE
)
FetchContent_MakeAvailable(freertos_kernel)

add_library(emblib_freertos INTERFACE)

# Link the FreeRTOS library
target_link_libraries(emblib_freertos
INTERFACE
    emblib
    freertos_kernel
)

# Provide header implementations
target_include_prefix(emblib_freertos
INTERFACE
    emblib/freertos ${CMAKE_CURRENT_SOURCE_DIR}/include
)
