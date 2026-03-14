add_library(emblib_posix
    src/sock_dev.cpp
    src/udp_dev.cpp
)

target_link_libraries(emblib_posix
PUBLIC
    emblib
)

target_include_prefix(emblib_posix
PUBLIC
    emblib/posix ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if (PROJECT_IS_TOP_LEVEL)
    add_executable(emblib_posix_tests
        test/udp_dev.test.cpp
    )

    target_link_libraries(emblib_posix_tests
    PRIVATE
        Catch2::Catch2WithMain
        emblib_posix
    )

    catch_discover_tests(emblib_posix_tests)
endif()
