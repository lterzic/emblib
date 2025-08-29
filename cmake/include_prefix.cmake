function(target_include_prefix target visibility prefix path)
    # Prefix relative path without the last entry since
    # the last entry will be the symlink to the original
    # include folder
    get_filename_component(PREFIX_DIR ${prefix} DIRECTORY)

    # Create symlinks in the include folder of the current CMake
    # build directory
    set(BUILD_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/include)
    set(SYMLINK_PATH ${BUILD_INCLUDE_DIR}/${prefix})

    file(MAKE_DIRECTORY ${BUILD_INCLUDE_DIR}/${PREFIX_DIR})
    file(CREATE_LINK ${path} ${SYMLINK_PATH} SYMBOLIC)

    target_include_directories(${target} ${visibility} ${BUILD_INCLUDE_DIR})
endfunction()