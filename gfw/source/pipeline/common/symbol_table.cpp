#include "gfw/pipeline/common/parse_tree.h"
#include "gfw/pipeline/common/symbol_table.h"

namespace GFW {

    bool SymbolTable::CollectSymbol( ConstParseTreeIn tree )
    {
        return true;
    }

    SymbolRef SymbolTable::LookupSymbol( const char * name )
    {
        return NULL;
    }

} // namesspace GFW
