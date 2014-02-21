#ifndef __GFW_PIPELINE_COMMON_SYMBOL_TABLE_H__
#define __GFW_PIPELINE_COMMON_SYMBOL_TABLE_H__

#include "gfw/pipeline/common/parse_tree.h"

namespace GFW {

    class Symbol : public Common::ARefCounted
    {
    public:
        const char *
            GetName() { return mName; }

    private:
        ConstParseTreeRef mTree;
        const char *      mName;
        uint32_t          mFlags;

        friend class SymbolTable;
    };
    AUTOREF_REFERENCE_DECLARATION( Symbol );

    class SymbolTable : public Common::ARefCounted
    {
    public:
        bool
        CollectSymbol( ConstParseTreeIn );

        SymbolRef
        LookupSymbol( const char * name );

    private:
        typedef std::vector< SymbolRef > SymbolVec;

        SymbolVec mSymbols;
    };
    AUTOREF_REFERENCE_DECLARATION( SymbolTable );

} // namespace GFW

#endif // __GFW_PIPELINE_COMMON_SYMBOL_TABLE_H__
