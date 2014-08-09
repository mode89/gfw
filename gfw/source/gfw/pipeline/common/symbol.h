#ifndef __GFW_PIPELINE_COMMON_SYMBOL_H__
#define __GFW_PIPELINE_COMMON_SYMBOL_H__

#include "gfw/pipeline/common/parse_tree.h"

#include <vector>

#define SYMBOL_FLAGS \
    F( Function ) \
    F( StateObject ) \
    F( Struct ) \
    F( Variable ) \

namespace GFW {

    class Symbol;
    typedef std::vector< const Symbol * > SymbolReferenceVec;

    class Symbol
    {
        typedef std::vector< const ParseTree * > ParseTreeVec;

    public:
        enum RegisterType
        {
            REGISTER_TYPE_UNKNOWN = 0,
            REGISTER_TYPE_CBUFFER,
            REGISTER_TYPE_TEXTURE,
            REGISTER_TYPE_SAMPLER,
            REGISTER_TYPE_UAV,
        };

        Symbol()
            : mTree( nullptr )
            , mName( nullptr )
            , mFlags( 0 )
            , mType( nullptr )
            , mSemantic( nullptr )
            , mRegisterType( REGISTER_TYPE_UNKNOWN )
            , mRegisterIndex( 0 )
        {}

        Symbol( const ParseTree & tree )
            : mTree( &tree )
            , mName( nullptr )
            , mFlags( 0 )
            , mType( nullptr )
            , mSemantic( nullptr )
            , mRegisterType( REGISTER_TYPE_UNKNOWN )
            , mRegisterIndex( 0 )
        {
            const ParseTree * symbolName = tree.GetFirstChildWithType( TOKEN_SYMBOL_NAME );
            mName = symbolName ? &symbolName->GetChild().GetText() : nullptr;
        }

        const std::string &
        GetName() const { return *mName; }

        TokenType
        GetTokenType() const { return mTree->GetTokenType(); }

        const ParseTree &
        GetTree() const { return *mTree; }

        // Symbols referenced by this symbol (i.e. global variables by a function)
        const SymbolReferenceVec &
        GetReferences() const { return mReferences; }

        // Object type or function return type
        const ParseTree &
        GetType() const { return *mType; }

        // Function arguments
        const ParseTreeVec &
        GetArgs() const { return mArgs; }

        // Members of a struct
        const ParseTreeVec &
        GetMembers() const { return mMembers; }

        const std::string &
        GetSemantic() const { return *mSemantic; }

        bool
        RefersTo( const Symbol * ) const;

#define F( name ) bool Is ## name () const { return ( mFlags & ( 1 << name ) ) != 0; }
        SYMBOL_FLAGS
#undef F

    private:

#define F( name ) void Set ## name () { mFlags |= ( 1 << name ); };
        SYMBOL_FLAGS
#undef F

    private:
        enum Flags
        {
            Unknown = -1,
#define F( name ) name,
            SYMBOL_FLAGS
#undef F
        };

        const ParseTree *   mTree;
        const std::string * mName;
        uint32_t            mFlags;
        SymbolReferenceVec  mReferences;    // Symbols referenced by this symbol (i.e. global variables by a function)
        const ParseTree *   mType;          // Object type or function return type
        ParseTreeVec        mArgs;          // Function arguments
        ParseTreeVec        mMembers;       // Members of a struct
        const std::string * mSemantic;
        RegisterType        mRegisterType;
        uint32_t            mRegisterIndex;

        friend class SymbolTable;
    };

} // namespace GFW

#endif // __GFW_PIPELINE_COMMON_SYMBOL_H__
