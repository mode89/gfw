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
    public:
        Symbol()
            : mName( NULL )
            , mFlags( 0 )
            , mSemantic( NULL )
        {}

        Symbol( ConstParseTreeIn tree )
            : mName( NULL )
            , mTree( tree )
            , mFlags( 0 )
            , mSemantic( NULL )
        {
            ConstParseTreeRef symbolName = tree->GetFirstChildWithType( TOKEN_SYMBOL_NAME );
            mName = symbolName.IsAttached() ? symbolName->GetChild()->ToString() : NULL;
        }

        const char *
        GetName() const { return mName; }

        TokenType
        GetTokenType() const { return mTree->GetTokenType(); }

        ConstParseTreeRef
        GetTree() const { return mTree; }

        // Symbols referenced by this symbol (i.e. global variables by a function)
        const SymbolReferenceVec &
        GetReferences() const { return mReferences; }

        // Object type or function return type
        ConstParseTreeRef
        GetType() const { return mType; }

        // Function arguments
        const ParseTreeVec &
        GetArgs() const { return mArgs; }

        // Members of a struct
        const ParseTreeVec &
        GetMembers() const { return mMembers; }

        const char *
        GetSemantic() const { return mSemantic; }

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

        ConstParseTreeRef   mTree;
        const char *        mName;
        uint32_t            mFlags;
        SymbolReferenceVec  mReferences;    // Symbols referenced by this symbol (i.e. global variables by a function)
        ConstParseTreeRef   mType;          // Object type or function return type
        ParseTreeVec        mArgs;          // Function arguments
        ParseTreeVec        mMembers;       // Members of a struct
        const char *        mSemantic;

        friend class SymbolTable;
    };

} // namespace GFW

#endif // __GFW_PIPELINE_COMMON_SYMBOL_H__
