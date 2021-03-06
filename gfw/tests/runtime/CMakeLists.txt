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

    add_executable( gfw-runtime-tests ${GFW_RUNTIME_TESTS_SOURCE_FILES} ${GFW_TESTS_FX_FILES} )
    target_link_libraries( gfw-runtime-tests gfw-runtime )

    add_test(
        NAME GfwRuntimeTests
        COMMAND gfw-runtime-tests --framecount 1 --compare --nowindow )

# Dependencies

    find_package( cmn CONFIG )
    target_link_libraries( gfw-runtime-tests ${CMN_LIBS} )
    include_directories( ${CMN_INCLUDE_DIRS} )

    find_package( gtest CONFIG )
    target_link_libraries( gfw-runtime-tests ${GTEST_LIBS} )
    include_directories( ${GTEST_INCLUDE_DIRS} )

    target_link_libraries( gfw-runtime-tests ${PNG_LIBS} )
    include_directories( ${PNG_INCLUDE_DIRS} )

# Post build

    if( NOT DEFINED GFW_RUNTIME_TESTS_POST_BUILD )
        if( CMAKE_GENERATOR_TOOLSET )
            set( TOOLSET_OPTION -T ${CMAKE_GENERATOR_TOOLSET} )
        endif()

        # Invoke post-build step
        add_custom_command(
            TARGET gfw-runtime-tests POST_BUILD
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
        # Generate build commands for effect files

            file( GLOB FX_FILES ${GFW_TESTS_DATA_DIR}/*.fx )
            foreach( file ${FX_FILES} )
                get_filename_component( fileName ${file} NAME )
                set( outputFileName "${GFW_RUNTIME_TESTS_OUTPUT_DIR}/${fileName}c" )
                add_custom_command(
                    OUTPUT ${outputFileName}
                    COMMAND gfwc -fx "${file}" -o ${outputFileName}
                    DEPENDS "${file}" )
                list( APPEND DEPS ${outputFileName} )
            endforeach()

        # Generate build commands for scene files

            file( GLOB DAE_FILES ${GFW_TESTS_DATA_DIR}/*.dae )
            foreach( file ${DAE_FILES} )
                get_filename_component( fileName ${file} NAME_WE )
                set( outputFileName "${GFW_RUNTIME_TESTS_OUTPUT_DIR}/${fileName}.sb" )
                add_custom_command(
                    OUTPUT ${outputFileName}
                    COMMAND gfwc -s "${file}" -o ${outputFileName}
                    DEPENDS "${file}" )
                list( APPEND DEPS ${outputFileName} )
            endforeach()

        # Post-build target

            add_custom_target( gfw-runtime-tests-postbuild DEPENDS ${DEPS} )
    endif()
