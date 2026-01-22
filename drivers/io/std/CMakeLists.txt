add_library(emblib_io_std INTERFACE)

target_link_libraries(emblib_io_std
INTERFACE
    emblib
)

target_include_prefix(emblib_io_std
INTERFACE
    emblib/io/std ${CMAKE_CURRENT_SOURCE_DIR}/include
)
