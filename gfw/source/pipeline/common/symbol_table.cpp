#include "gfw/pipeline/common/parse_tree.h"
#include "gfw/pipeline/common/symbol_table.h"

#include <algorithm>
#include <cstring>

namespace GFW {

    static bool LessByName( const Symbol & l, const Symbol & r )
    {
        return std::strcmp( l.GetName(), r.GetName() ) < 0;
    }

    bool SymbolTable::CollectSymbol( ConstParseTreeIn tree )
    {
        switch ( tree->GetTokenType() )
        {
        case TOKEN_VARIABLE_DEFINITION:
            {
                Symbol symbol( tree );
                symbol.SetVariable();
                AddSymbol( symbol );
            }
            return false;
        case TOKEN_FUNCTION_DEFINITION:
            {
                Symbol symbol( tree );
                symbol.SetFunction();
                AddSymbol( symbol );
            }
            return false;
        case TOKEN_STATE_OBJECT_DEFINITION:
            {
                Symbol symbol( tree );
                symbol.SetStateObject();
                symbol.SetVariable();
                AddSymbol( symbol );
            }
            return false;
        }
        return true;
    }

    const Symbol * SymbolTable::LookupSymbol( const char * name ) const
    {
        Symbol symbol;
        symbol.mName = name;

        SymbolVec::const_iterator it = lower_bound( mSymbols.begin(), mSymbols.end(), symbol, LessByName );
        if ( it != mSymbols.end() )
        {
            return &(*it);
        }

        return NULL;
    }

    void SymbolTable::AddSymbol( const Symbol & symbol )
    {
        mSymbols.push_back( symbol );
        std::sort( mSymbols.begin(), mSymbols.end(), LessByName );
    }

} // namesspace GFW
