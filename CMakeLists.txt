cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(emblib VERSION 1.0)


# Use the default emblib FreeRTOS configuration if one not provided
if (NOT TARGET freertos_config)
    message(STATUS "emblib: FreeRTOS configuration not set, using emblib default FreeRTOS config")
    add_library(freertos_config INTERFACE)
    target_include_directories(freertos_config SYSTEM INTERFACE src)
endif()

target_compile_definitions(freertos_config INTERFACE
    projCOVERAGE_TEST=0
    projENABLE_TRACING=0
)


# If there is FreeRTOS in a parent project, use that, else use submodule
if (TARGET freertos_kernel)
    message(STATUS "emblib: FreeRTOS inherited from a parent project")
else()
    message(STATUS "emblib: Using FreeRTOS submodule from emblib")
    add_subdirectory("lib/FreeRTOS-Kernel")
endif()

if (PROJECT_IS_TOP_LEVEL OR NOT TARGET freertos_hooks)
    # Stub implementation for FreeRTOS hooks
    target_sources(freertos_kernel PRIVATE src/rtos/freertos/hooks.c)
else()
    target_link_libraries(freertos_kernel PRIVATE freertos_hooks)
endif()


# GLM
set(GLM_BUILD_LIBRARY OFF)
set(GLM_BUILD_INSTALL OFF)
set(GLM_ENABLE_CXX_17 ON)


message(STATUS "emblib: GLM and ETL need to be built and installed, follow GitHub readmes for info")
add_subdirectory("lib/etl")
add_subdirectory("lib/glm")
add_subdirectory("lib/eigen")

# Testing only if this is the top level project
if (PROJECT_IS_TOP_LEVEL)
    add_subdirectory("lib/Catch2")
    add_subdirectory("test")
endif()


# Create the library target with all the source files
add_library(emblib STATIC
    src/main.cpp
)

# Add include folders
target_include_directories(emblib PUBLIC
    "${PROJECT_SOURCE_DIR}/include"
)

# If this is the top level project then use own config file
# else import it from the parent project
if (PROJECT_IS_TOP_LEVEL)
    target_include_directories(emblib PUBLIC
        "${PROJECT_SOURCE_DIR}/src"
    )
else()
    if (TARGET emblib_config)
        target_link_libraries(emblib PUBLIC emblib_config)
    else()
        message(FATAL_ERROR "emblib: config not found, create an interface library"
        "and add include directory which holds the emblib config file:"
        "add_library(emblib_config INTERFACE)"
        "target_include_directories(emblib_config INTERFACE <path>)")
    endif()
endif()


# Link all sublibraries
target_link_libraries(emblib PUBLIC
    freertos_kernel
    etl::etl
    glm::glm-header-only
    eigen
)

# Remove RTTI and exceptions
target_compile_options(emblib PUBLIC
    -fno-rtti
    -fno-exceptions
)

# Executable for testing on the fly
if (PROJECT_IS_TOP_LEVEL)
    # Flags for the build
    set(CMAKE_BUILD_TYPE Debug)
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
    set(CMAKE_CXX_FLAGS_RELEASE "-O2")

    # Executable for running custom testing and debugging
    add_executable(emblib_test src/test.cpp)
    target_link_libraries(emblib_test PRIVATE emblib)
endif()

# TODO: Add a custom command for testing (cmake test)