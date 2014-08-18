project( GFW_RUNTIME_TESTS )
cmake_minimum_required( VERSION 2.8 )

# Source files

    file( GLOB TEMP_LIST *.cpp )
    source_group( "source" FILES ${TEMP_LIST} )
    list( APPEND GFW_RUNTIME_TESTS_SOURCE_FILES ${TEMP_LIST} )

    source_group( "fx" FILES ${GFW_TESTS_FX_FILES} )

# Includes

    include_directories(
        ${GFW_ROOT}/include
    )

# Defines

    add_definitions( -DGFW_TESTS_RUNTIME_REFS_DIR="${CMAKE_CURRENT_LIST_DIR}/refs" )

# Target

    add_executable( gfw_runtime-tests ${GFW_RUNTIME_TESTS_SOURCE_FILES} ${GFW_TESTS_FX_FILES} )
    target_link_libraries( gfw_runtime-tests gfw_runtime )

# Dependencies

    find_package( cmn CONFIG )
    target_link_libraries( gfw_runtime-tests ${CMN_LIBS} )
    include_directories( ${CMN_INCLUDE_DIRS} )

    find_package( gtest CONFIG )
    target_link_libraries( gfw_runtime-tests ${GTEST_LIBS} )
    include_directories( ${GTEST_INCLUDE_DIRS} )

    target_link_libraries( gfw_runtime-tests ${PNG_LIBS} )
    include_directories( ${PNG_INCLUDE_DIRS} )

# Post build

    if( NOT DEFINED GFW_RUNTIME_TESTS_POST_BUILD )
        if( CMAKE_GENERATOR_TOOLSET )
            set( TOOLSET_OPTION -T ${CMAKE_GENERATOR_TOOLSET} )
        endif()

        # Invoke post-build step
        add_custom_command(
            TARGET gfw_runtime-tests POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory post-build
            COMMAND ${CMAKE_COMMAND} -E chdir post-build ${CMAKE_COMMAND}
                -DGFW_RUNTIME_TESTS_POST_BUILD=1
                -DGFW_RUNTIME_TESTS_OUTPUT_DIR="${CMAKE_CURRENT_BINARY_DIR}"
                -DCMAKE_BUILD_TYPE=Release
                -G ${CMAKE_GENERATOR}
                ${TOOLSET_OPTION}
                ${CMAKE_SOURCE_DIR}
            COMMAND ${CMAKE_COMMAND} -E chdir post-build ${CMAKE_COMMAND}
                --build .
                --target gfw-runtime-tests-postbuild
                --config Release
            COMMENT "Post-build gfw-runtime-tests"
        )
    else()
        # Post-build target
        add_custom_target( gfw-runtime-tests-postbuild
            gfw_fxc
            -fx "${GFW_TESTS_DATA_DIR}/draw.fx"
            -o "${GFW_RUNTIME_TESTS_OUTPUT_DIR}/draw.fxc"
        )
    endif()
