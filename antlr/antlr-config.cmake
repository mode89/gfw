list( APPEND ANTLR_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/include )
list( APPEND ANTLR_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/include/antlr )
set( ANTLR_LIBRARIES antlr )
set( ANTLR_TOOL ${CMAKE_CURRENT_LIST_DIR}/bin/antlr-3.4-complete-no-antlrv2.jar )
set( ANTLR_TOK2DEF ${CMAKE_CURRENT_LIST_DIR}/scripts/tok2def.py )

function( antlr_prebuild_grammar target grammar )
    add_custom_command( TARGET ${target} PRE_BUILD COMMAND
        java ARGS -jar ${ANTLR_TOOL} -make -language C ${grammar} )
endfunction()
