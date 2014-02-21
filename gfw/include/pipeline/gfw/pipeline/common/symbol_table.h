#ifndef __GFW_PIPELINE_COMMON_SYMBOL_TABLE_H__
#define __GFW_PIPELINE_COMMON_SYMBOL_TABLE_H__

#include "gfw/pipeline/common/parse_tree.h"

namespace GFW {

#define SYMBOL_FLAGS \
    F( Function ) \
    F( StateObject ) \
    F( Variable ) \

    class Symbol
    {
    public:
        Symbol()
            : mName( NULL )
            , mFlags( 0 )
        {}

        Symbol( ConstParseTreeIn tree )
            : mName( NULL )
            , mTree( tree )
            , mFlags( 0 )
        {
            ConstParseTreeRef symbolName = tree->GetFirstChildWithType( TOKEN_SYMBOL_NAME );
            mName = symbolName->GetChild()->ToString();
        }

        const char *
        GetName() const { return mName; }

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

        ConstParseTreeRef mTree;
        const char *      mName;
        uint32_t          mFlags;

        friend class SymbolTable;
    };

    class SymbolTable : public Common::ARefCounted
    {
    public:
        bool
        CollectSymbol( ConstParseTreeIn );

        const Symbol *
        LookupSymbol( const char * name ) const;

    private:
        void
        AddSymbol( const Symbol & );

    private:
        typedef std::vector< Symbol > SymbolVec;

        SymbolVec mSymbols;
    };
    AUTOREF_REFERENCE_DECLARATION( SymbolTable );

} // namespace GFW

#endif // __GFW_PIPELINE_COMMON_SYMBOL_TABLE_H__
