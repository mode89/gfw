#ifndef __GFW_PIPELINE_COMMON_SYMBOL_H__
#define __GFW_PIPELINE_COMMON_SYMBOL_H__

#include "gfw/pipeline/common/parse_tree.h"

#include <vector>

namespace GFW {

    struct Symbol;
    typedef std::vector< const Symbol * > SymbolReferenceVec;

    struct Symbol
    {
        typedef std::vector< const ParseTree * > ParseTreeVec;

        enum RegisterType
        {
            REGISTER_TYPE_UNKNOWN = 0,
            REGISTER_TYPE_CBUFFER,
            REGISTER_TYPE_TEXTURE,
            REGISTER_TYPE_SAMPLER,
            REGISTER_TYPE_UAV,
        };

        bool
        RefersTo( const Symbol * ) const;

        const std::string * name;
        const ParseTree *   tree;
        SymbolReferenceVec  references;    // Symbols referenced by this symbol (i.e. global variables by a function)
        const ParseTree *   type;          // Object type or function return type
        ParseTreeVec        args;          // Function arguments
        ParseTreeVec        members;       // Members of a struct
        const std::string * semantic;
        RegisterType        registerType;
        uint32_t            registerIndex;

        bool                isFunction : 1;
        bool                isStateObject : 1;
        bool                isStruct : 1;
        bool                isVariable : 1;

        Symbol()
            : name( nullptr )
            , tree( nullptr )
            , references()
            , type( nullptr )
            , args()
            , members()
            , semantic( nullptr )
            , registerType( REGISTER_TYPE_UNKNOWN )
            , registerIndex( 0 )
            , isFunction( false )
            , isStateObject( false )
            , isStruct( false )
            , isVariable( false )
        {}
    };

} // namespace GFW

#endif // __GFW_PIPELINE_COMMON_SYMBOL_H__
