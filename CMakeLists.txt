cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(emblib VERSION 1.0)

# Create a static library with a dummy source file
add_library(emblib INTERFACE)

# Add include folders
target_include_directories(emblib
INTERFACE
    "${PROJECT_SOURCE_DIR}/include"
)

# Libraries used by emblib
include(FetchContent)

FetchContent_Declare(
    etl
    GIT_REPOSITORY  https://github.com/ETLCPP/etl.git
    GIT_TAG         20.44.1
    GIT_SHALLOW     TRUE
)
FetchContent_MakeAvailable(etl)

FetchContent_Declare(
    units
    GIT_REPOSITORY  https://github.com/nholthaus/units.git
    GIT_TAG         v2.3.5
    GIT_SHALLOW     TRUE
)
FetchContent_MakeAvailable(units)

# Libary options
target_compile_definitions(emblib
INTERFACE
    UNIT_LIB_DISABLE_IOSTREAM
    DISABLE_PREDEFINED_UNITS
)

# Link all sublibraries
target_link_libraries(emblib
INTERFACE
    etl::etl
    units
)

# Testing environment only if top level project
if (PROJECT_IS_TOP_LEVEL)
    add_subdirectory("test")
    # TODO: Add a custom command for testing (cmake test)
endif()
