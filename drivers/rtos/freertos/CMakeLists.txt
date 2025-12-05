FetchContent_Declare(
    freertos_kernel
    GIT_REPOSITORY  https://github.com/FreeRTOS/FreeRTOS-Kernel.git
    GIT_TAG         V11.2.0
    GIT_SHALLOW     TRUE
)
FetchContent_MakeAvailable(freertos_kernel)

add_library(emblib_rtos_freertos INTERFACE)

# Link the FreeRTOS library
target_link_libraries(emblib_rtos_freertos
INTERFACE
    emblib
    freertos_kernel
)

# Provide header implementations
# TODO: This should be PRIVATE somehow (not directly used)
target_include_directories(emblib_rtos_freertos
INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)