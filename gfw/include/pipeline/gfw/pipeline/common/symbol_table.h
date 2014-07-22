#ifndef __GFW_PIPELINE_COMMON_SYMBOL_TABLE_H__
#define __GFW_PIPELINE_COMMON_SYMBOL_TABLE_H__

#include "gfw/pipeline/common/symbol.h"

namespace GFW {

    class SymbolTable
    {
        typedef std::vector< Symbol > SymbolVec;

    public:
        typedef SymbolVec::const_iterator const_iterator;

        SymbolTable( const ParseTree & );

        bool
        CollectSymbol( const ParseTree & );

        bool
        LookupSymbolByName( const std::string & name, SymbolReferenceVec & result ) const;

        const Symbol *
        LookupSymbolByTree( const ParseTree & ) const;

        const_iterator begin() const { return mSymbols.begin(); }

        const_iterator end() const { return mSymbols.end(); }

    private:
        void
        AddSymbol( const Symbol & );

    private:
        SymbolVec           mSymbols;
        SymbolReferenceVec  mSymbolsByName;
        SymbolReferenceVec  mSymbolsByTreeAddress;
    };

} // namespace GFW

#endif // __GFW_PIPELINE_COMMON_SYMBOL_TABLE_H__
