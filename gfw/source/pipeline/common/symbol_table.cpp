#include "gfw/pipeline/common/parse_tree.h"
#include "gfw/pipeline/common/symbol_table.h"

#include <algorithm>
#include <cstring>

namespace GFW {

    static bool LessByName( const Symbol & l, const Symbol & r )
    {
        return std::strcmp( l.GetName(), r.GetName() ) < 0;
    }

    class CollectReferencedSymbolsVisitor
    {
    public:
        CollectReferencedSymbolsVisitor( const SymbolTable * symbolTable, Symbol::References & references )
            : mSymbolTable( symbolTable )
            , mReferences( references )
        {}

        bool operator() ( ConstParseTreeIn tree )
        {
            if ( tree->GetTokenType() == TOKEN_ID )
            {
                mSymbolTable->LookupSymbol( tree->ToString(), mReferences );
            }
            return true;
        }

    private:
        const SymbolTable *  mSymbolTable;
        Symbol::References & mReferences;
    };

    SymbolTable::SymbolTable( ConstParseTreeIn tree )
    {
        tree->TraverseDFS( *this, &SymbolTable::CollectSymbol );

        // Collect immediate references (variable by function, function by function, etc)

        for ( SymbolVec::iterator it = mSymbols.begin(); it != mSymbols.end(); ++ it )
        {
            Symbol & symbol = *it;
            symbol.mReferences.push_back( &symbol );
            switch ( it->GetTokenType() )
            {
            case TOKEN_FUNCTION_DEFINITION:
                {
                    ConstParseTreeRef functionBody = symbol.GetTree()->GetFirstChildWithType( TOKEN_COMPOUND_STATEMENT );
                    CollectReferencedSymbolsVisitor collectReferencedSymbolsVisitor( this, symbol.mReferences );
                    functionBody->TraverseDFS( collectReferencedSymbolsVisitor );
                }
                break;
            }
        }

        // Calculate indirect references
        //
        // First, find immediate references for the immediate references of the current symbol - 
        // all new references (that are not immediate to the current symbol0) add to the references
        // of the current symbol. Then find all immediate references for these new references, and
        // process all new references the same way as the first new references (add to the current
        // symbol, find immediate references for them). Do it until there is no more new references.

        for ( SymbolVec::iterator it = mSymbols.begin(); it != mSymbols.end(); ++ it )
        {
            Symbol & symbol = *it;

            // Start with immediate references
            Symbol::References newReferences = symbol.mReferences;
            while (!newReferences.empty())
            {
                // Sort known references to be able to search through them
                std::sort( symbol.mReferences.begin(), symbol.mReferences.end() );

                // Enumerate through new references
                Symbol::References nextNewReferences;
                for ( Symbol::References::iterator processingIt = newReferences.begin(); processingIt != newReferences.end(); ++ processingIt )
                {
                    // Enumerate through immediate references of the new references
                    Symbol::References indirectReferences = (*processingIt)->mReferences;
                    for ( Symbol::References::iterator indirectIt = indirectReferences.begin(); indirectIt != indirectReferences.end(); ++ indirectIt )
                    {
                        // If the reference is not known then we will process it during the next iteration
                        if ( !std::binary_search(symbol.mReferences.begin(), symbol.mReferences.end(), *indirectIt) )
                        {
                            nextNewReferences.push_back( *indirectIt );
                        }
                    }
                }
                newReferences = nextNewReferences;

                // Add new references to the known references
                symbol.mReferences.insert( symbol.mReferences.end(), nextNewReferences.begin(), nextNewReferences.end() );
            }
        }
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

    void SymbolTable::LookupSymbol( const char * name, Symbol::References & result ) const
    {
        Symbol symbol;
        symbol.mName = name;

        SymbolVec::const_iterator it = lower_bound( mSymbols.begin(), mSymbols.end(), symbol, LessByName );
        if ( it != mSymbols.end() )
        {
            for (; it != mSymbols.end() && std::strcmp( it->GetName(), name ) == 0 ; ++ it )
            {
                result.push_back( &(*it) );
            }
        }
    }

    void SymbolTable::AddSymbol( const Symbol & symbol )
    {
        mSymbols.push_back( symbol );
        std::sort( mSymbols.begin(), mSymbols.end(), LessByName );
    }

} // namesspace GFW
