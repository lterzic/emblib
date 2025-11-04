# Macro to init and add specific submodule
#
# Note: Using function instead of macro doesn't
# work with Catch2
macro(add_submodule path)
    message(STATUS "Adding submodule: ${path}")
    execute_process(COMMAND git submodule update --init --recursive ${path}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                    RESULT_VARIABLE result
                    OUTPUT_VARIABLE output
                    ERROR_VARIABLE error)
    
    # Provide the binary dir if required as the optional argument
    add_subdirectory(${path} ${ARGV1})

    if(NOT result EQUAL 0)
        message(FATAL_ERROR "Failed to initialize submodule: ${path}\n${output}\n${error}")
    endif()
endmacro()