#ifndef __GFW_PIPELINE_COMMON_SYMBOL_TABLE_H__
#define __GFW_PIPELINE_COMMON_SYMBOL_TABLE_H__

#include "gfw/pipeline/common/symbol.h"

namespace GFW {

    class SymbolTable : public Common::ARefCounted
    {
        typedef std::vector< Symbol > SymbolVec;

    public:
        typedef SymbolVec::const_iterator const_iterator;

        SymbolTable( ConstParseTreeIn );

        bool
        CollectSymbol( ConstParseTreeIn );

        bool
        LookupSymbolByName( const char * name, Symbol::References & result ) const;

        const Symbol *
        LookupSymbolByTree( ConstParseTreeIn ) const;

        const_iterator begin() const { return mSymbols.begin(); }

        const_iterator end() const { return mSymbols.end(); }

    private:
        void
        AddSymbol( const Symbol & );

    private:
        SymbolVec           mSymbols;
        Symbol::References  mSymbolsByName;
        Symbol::References  mSymbolsByTreeAddress;
    };
    AUTOREF_REFERENCE_DECLARATION( SymbolTable );

} // namespace GFW

#endif // __GFW_PIPELINE_COMMON_SYMBOL_TABLE_H__
