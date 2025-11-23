add_library(emblib_rtos_freertos INTERFACE)

# If there is FreeRTOS in a parent project, use that, else use submodule
if (TARGET freertos_kernel)
    message(STATUS "emblib: FreeRTOS inherited from a parent project")
else()
    message(STATUS "emblib: Using the FreeRTOS submodule from emblib, to "
    "use a different source for FreeRTOS define a freertos_kernel target "
    "in a parent project")
    add_submodule("lib/FreeRTOS-Kernel")
endif()

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