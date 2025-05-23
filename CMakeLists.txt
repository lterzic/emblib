cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(emblib VERSION 1.0)

# Create a static library with a dummy source file
add_library(emblib STATIC
    src/main.cpp
)

# Add include folders
target_include_directories(emblib PUBLIC
    "${PROJECT_SOURCE_DIR}/include"
)

# Useful compiler and linker options for embedded environments
# Even though this is header only, submodules (like FreeRTOS
# or Eigen) can benefit from these
target_compile_options(emblib PRIVATE
    -fno-rtti
    -fno-exceptions
    -fdata-sections
    -ffunction-sections
)
target_link_options(emblib PRIVATE
    -Wl,-Map=${CMAKE_PROJECT_NAME}.map
    -Wl,--gc-sections
    -nostdlib++
)

# Libraries used by emblib
include("lib/addsubmodule.cmake")
add_submodule("lib/etl")
add_submodule("lib/eigen")
add_submodule("lib/units")

# Libary options
target_compile_definitions(emblib PUBLIC
    EIGEN_NO_MALLOC
    EIGEN_NO_IO
    UNIT_LIB_DISABLE_IOSTREAM
    DISABLE_PREDEFINED_UNITS
)

# Link all sublibraries
target_link_libraries(emblib PUBLIC
    freertos_kernel
    etl::etl
    eigen
    units
)

# If this is the top level project then use default configurations
# else import them from the parent projects
if (PROJECT_IS_TOP_LEVEL)
    # Use the default emblib config
    target_include_directories(emblib PUBLIC ${PROJECT_SOURCE_DIR}/src)
    # Use the default FreeRTOS configuration
    add_library(freertos_config INTERFACE)
    target_include_directories(freertos_config SYSTEM INTERFACE ${PROJECT_SOURCE_DIR}/src)
    target_compile_definitions(freertos_config INTERFACE
        projCOVERAGE_TEST=0
        projENABLE_TRACING=0
    )
else()
    if (TARGET emblib_config)
        target_link_libraries(emblib PUBLIC emblib_config)
    else()
        message(FATAL_ERROR "emblib: Config not found, create an interface library"
        "and add include directory which holds the emblib config file:"
        "add_library(emblib_config INTERFACE)"
        "target_include_directories(emblib_config INTERFACE <path>)")
    endif()
endif()

# If there is FreeRTOS in a parent project, use that, else use submodule
if (TARGET freertos_kernel)
    message(STATUS "emblib: FreeRTOS inherited from a parent project")
else()
    message(WARNING "emblib: Using the FreeRTOS submodule from emblib, to "
    "use a different source for FreeRTOS define a freertos_kernel target "
    "in a parent project")
    add_submodule("lib/FreeRTOS-Kernel")
endif()

# Testing environment only if top level project
if (PROJECT_IS_TOP_LEVEL)
    # Add the testing library and all the tests
    add_submodule("lib/Catch2")
    add_subdirectory("test")

    # Flags for the build
    target_compile_options(emblib PUBLIC -Wall -Wextra -Wpedantic)
    set(CMAKE_BUILD_TYPE Debug)
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

    # Executable for running custom testing and debugging
    add_executable(emblib_test src/test.cpp)
    target_link_libraries(emblib_test PRIVATE emblib)
endif()

# TODO: Add a custom command for testing (cmake test)