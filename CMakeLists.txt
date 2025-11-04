cmake_minimum_required(VERSION 3.14)

# CMake custom scripts
include("cmake/add_submodule.cmake")
include("cmake/include_prefix.cmake")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(emblib VERSION 1.0)

# Create a static library with a dummy source file
add_library(emblib STATIC
    src/main.cpp
)

# Add include folders
target_include_directories(emblib
PUBLIC
    "${PROJECT_SOURCE_DIR}/include"
)

# Useful compiler and linker options for embedded environments
# Even though this is header only, submodules (like FreeRTOS
# or Eigen) can benefit from these
target_compile_options(emblib
PRIVATE
    -fno-rtti
    -fno-exceptions
    -fdata-sections
    -ffunction-sections
)
target_link_options(emblib
PRIVATE
    -Wl,-Map=${CMAKE_PROJECT_NAME}.map
    -Wl,--gc-sections
    -nostdlib++
)

# Libraries used by emblib
add_submodule("lib/etl")
add_submodule("lib/units")

# Libary options
target_compile_definitions(emblib
PRIVATE
    UNIT_LIB_DISABLE_IOSTREAM
    DISABLE_PREDEFINED_UNITS
)

# Link all sublibraries
target_link_libraries(emblib
PUBLIC
    etl::etl
    units
)

# Testing environment only if top level project
if (PROJECT_IS_TOP_LEVEL)
    add_subdirectory("test")
    # TODO: Add a custom command for testing (cmake test)
endif()
