#include "gfw/pipeline/common/parse_tree.h"
#include "gfw/pipeline/common/symbol_table.h"

#include <algorithm>
#include <cstring>

namespace GFW {

    static bool SymbolLessByName( const Symbol * l, const Symbol * r )
    {
        return std::strcmp( l->GetName(), r->GetName() ) < 0;
    }

    static bool SymbolLessByTreeAddress( const Symbol * l, const Symbol * r )
    {
        return l->GetTree().GetPointer() < r->GetTree().GetPointer();
    }

    class CollectReferencedSymbolsVisitor
    {
    public:
        CollectReferencedSymbolsVisitor( const SymbolTable * symbolTable, SymbolReferenceVec & references )
            : mSymbolTable( symbolTable )
            , mReferences( references )
        {}

        bool operator() ( ConstParseTreeIn tree )
        {
            if ( tree->GetTokenType() == TOKEN_ID )
            {
                mSymbolTable->LookupSymbolByName( tree->ToString(), mReferences );
            }
            return true;
        }

    private:
        const SymbolTable *  mSymbolTable;
        SymbolReferenceVec & mReferences;
    };

    SymbolTable::SymbolTable( ConstParseTreeIn tree )
    {
        tree->TraverseDFS( *this, &SymbolTable::CollectSymbol );

        // Mapping by name

        for ( SymbolVec::iterator it = mSymbols.begin(); it != mSymbols.end(); ++ it )
        {
            mSymbolsByName.push_back( &(*it) );
        }
        std::sort( mSymbolsByName.begin(), mSymbolsByName.end(), SymbolLessByName );

        // Mapping by tree address

        for ( SymbolVec::iterator it = mSymbols.begin(); it != mSymbols.end(); ++ it )
        {
            mSymbolsByTreeAddress.push_back( &(*it) );
        }
        std::sort( mSymbolsByTreeAddress.begin(), mSymbolsByTreeAddress.end(), SymbolLessByName );

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
            SymbolReferenceVec newReferences = symbol.mReferences;
            while (!newReferences.empty())
            {
                // Sort known references to be able to search through them
                std::sort( symbol.mReferences.begin(), symbol.mReferences.end() );

                // Enumerate through new references
                SymbolReferenceVec nextNewReferences;
                for ( SymbolReferenceVec::iterator processingIt = newReferences.begin(); processingIt != newReferences.end(); ++ processingIt )
                {
                    // Enumerate through immediate references of the new references
                    SymbolReferenceVec indirectReferences = (*processingIt)->mReferences;
                    for ( SymbolReferenceVec::iterator indirectIt = indirectReferences.begin(); indirectIt != indirectReferences.end(); ++ indirectIt )
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

                // Remember a return type

                symbol.mType = tree->GetChild();

                // Remember arguments

                ConstParseTreeRef args = tree->GetFirstChildWithType( TOKEN_ARGUMENTS_LIST );
                if ( args.IsAttached() )
                {
                    for ( uint32_t i = 0; i < args->GetChildCount(); ++ i )
                    {
                        if ( args->GetChild(i)->GetTokenType() == TOKEN_ARGUMENT )
                        {
                            symbol.mArgs.push_back( args->GetChild(i) );
                        }
                    }
                }

                // Remember an output semantic

                ConstParseTreeRef semantic = tree->GetFirstChildWithType( TOKEN_SEMANTIC );
                if ( semantic.IsAttached() )
                {
                    symbol.mSemantic = semantic->GetChild()->ToString();
                }

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

    bool SymbolTable::LookupSymbolByName( const char * name, SymbolReferenceVec & result ) const
    {
        Symbol symbol;
        symbol.mName = name;

        SymbolReferenceVec::const_iterator it = lower_bound( mSymbolsByName.begin(), mSymbolsByName.end(), &symbol, SymbolLessByName );
        if ( it != mSymbolsByName.end() )
        {
            for (; it != mSymbolsByName.end() && std::strcmp( (*it)->GetName(), name ) == 0 ; ++ it )
            {
                result.push_back( *it );
            }
        }

        return !result.empty();
    }

    const Symbol * SymbolTable::LookupSymbolByTree( ConstParseTreeIn tree ) const
    {
        Symbol symbol;
        symbol.mTree = tree;

        SymbolReferenceVec::const_iterator it = lower_bound( mSymbolsByTreeAddress.begin(), mSymbolsByTreeAddress.end(),
            &symbol, SymbolLessByTreeAddress );

        return ( it != mSymbolsByTreeAddress.end() ) ? *it : NULL;
    }

    void SymbolTable::AddSymbol( const Symbol & symbol )
    {
        mSymbols.push_back( symbol );
    }

} // namesspace GFW
