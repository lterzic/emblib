add_library(emblib_io_net
    src/udp_stream.cpp
)

target_link_libraries(emblib_io_net
PUBLIC
    emblib
)

target_include_prefix(emblib_io_net
PUBLIC
    emblib/io/net ${CMAKE_CURRENT_SOURCE_DIR}/include
)
