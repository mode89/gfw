#ifndef __GFW_PIPELINE_COMMON_SYMBOL_H__
#define __GFW_PIPELINE_COMMON_SYMBOL_H__

#include "gfw/pipeline/common/parse_tree.h"

#include <string>
#include <vector>
#include <unordered_map>

namespace GFW {

    struct Symbol;
    typedef std::list< Symbol > SymbolTable;
    typedef std::unordered_multimap< std::string, const Symbol * > NameSymbolMap;
    typedef std::unordered_map< const ParseTree *, const Symbol * > ParseTreeSymbolMap;

    void ConstructSymbolTable( SymbolTable &, const ParseTree & );

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

        const char *        name;
        const ParseTree *   tree;
        NameSymbolMap       references;     // All symbols referenced by this symbol (directly and indirectly)
        const ParseTree *   type;           // Object type or function return type
        ParseTreeVec        args;           // Function arguments
        ParseTreeVec        members;        // Members of a struct
        const Symbol *      parent;         // Parent of cbuffer's member
        const char *        semantic;
        RegisterType        registerType;
        int                 registerIndex;

        bool                isCbuffer : 1;
        bool                isCbufferMember : 1;
        bool                isFunction : 1;
        bool                isSamplerState : 1;
        bool                isStateObject : 1;
        bool                isStruct : 1;
        bool                isTexture : 1;
        bool                isVariable : 1;

        Symbol()
            : name( nullptr )
            , tree( nullptr )
            , references()
            , type( nullptr )
            , args()
            , members()
            , parent( nullptr )
            , semantic( nullptr )
            , registerType( REGISTER_TYPE_UNKNOWN )
            , registerIndex( -1 )
            , isCbuffer( false )
            , isCbufferMember( false )
            , isFunction( false )
            , isSamplerState( false )
            , isStateObject( false )
            , isStruct( false )
            , isTexture( false )
            , isVariable( false )
        {}
    };

} // namespace GFW

#endif // __GFW_PIPELINE_COMMON_SYMBOL_H__
