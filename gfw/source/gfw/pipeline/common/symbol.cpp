#include "cmn/trace.h"
#include "gfw/pipeline/common/parse_tree.h"
#include "gfw/pipeline/common/symbol_table.h"
#include <algorithm>
#include <cstring>

namespace GFW {

    static bool SymbolLessByName( const Symbol * l, const Symbol * r )
    {
        return *l->name < *r->name;
    }

    static bool SymbolLessByTreeAddress( const Symbol * l, const Symbol * r )
    {
        return l->tree < r->tree;
    }

    class CollectReferencedSymbolsVisitor
    {
    public:
        CollectReferencedSymbolsVisitor( const SymbolTable & symbolTable, SymbolReferenceVec & references )
            : mSymbolTable( symbolTable )
            , mReferences( references )
        {}

        bool operator() ( const ParseTree & tree )
        {
            if ( tree.GetTokenType() == TOKEN_ID )
            {
                mSymbolTable.LookupSymbolByName( tree.GetText(), mReferences );
            }
            return true;
        }

        CollectReferencedSymbolsVisitor operator= ( const CollectReferencedSymbolsVisitor & );

    private:
        const SymbolTable &  mSymbolTable;
        SymbolReferenceVec & mReferences;
    };

    bool Symbol::RefersTo( const Symbol * symbol ) const
    {
        return std::binary_search( references.begin(), references.end(), symbol );
    }

    SymbolTable::SymbolTable( const ParseTree & tree )
    {
        tree.TraverseDFS( *this, &SymbolTable::CollectSymbol );

        // Mapping by name
        for ( Symbol & symbol : mSymbols )
        {
            mSymbolsByName.push_back( &symbol );
        }
        std::sort( mSymbolsByName.begin(), mSymbolsByName.end(), SymbolLessByName );

        // Mapping by tree address
        for ( Symbol & symbol : mSymbols )
        {
            mSymbolsByTreeAddress.push_back( &symbol );
        }
        std::sort( mSymbolsByTreeAddress.begin(), mSymbolsByTreeAddress.end(), SymbolLessByTreeAddress );

        // Collect immediate references (variable by function, function by function, etc)

        for ( SymbolVec::iterator it = mSymbols.begin(); it != mSymbols.end(); ++ it )
        {
            Symbol & symbol = *it;
            symbol.references.push_back( &symbol );
            switch ( symbol.tree->GetTokenType() )
            {
            case TOKEN_FUNCTION_DEFINITION:
                {
                    CollectReferencedSymbolsVisitor collectReferencedSymbolsVisitor( *this, symbol.references );
                    symbol.tree->TraverseDFS( collectReferencedSymbolsVisitor );
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
            SymbolReferenceVec newReferences = symbol.references;
            while (!newReferences.empty())
            {
                // Sort known references to be able to search through them
                std::sort( symbol.references.begin(), symbol.references.end() );

                // Enumerate through new references
                SymbolReferenceVec nextNewReferences;
                for ( SymbolReferenceVec::iterator processingIt = newReferences.begin(); processingIt != newReferences.end(); ++ processingIt )
                {
                    // Enumerate through immediate references of the new references
                    SymbolReferenceVec indirectReferences = (*processingIt)->references;
                    for ( SymbolReferenceVec::iterator indirectIt = indirectReferences.begin(); indirectIt != indirectReferences.end(); ++ indirectIt )
                    {
                        // If the reference is not known then we will process it during the next iteration
                        if ( !std::binary_search(symbol.references.begin(), symbol.references.end(), *indirectIt) )
                        {
                            nextNewReferences.push_back( *indirectIt );
                        }
                    }
                }
                newReferences = nextNewReferences;

                // Add new references to the known references
                symbol.references.insert( symbol.references.end(), nextNewReferences.begin(), nextNewReferences.end() );
            }

            // Sort references to be able to search through them
            std::sort( symbol.references.begin(), symbol.references.end() );
        }
    }

    bool SymbolTable::CollectSymbol( const ParseTree & tree )
    {
        switch ( tree.GetTokenType() )
        {
        case TOKEN_VARIABLE_DEFINITION:
            {
                Symbol symbol;

                symbol.tree = &tree;
                const ParseTree * symbolName = tree.GetFirstChildWithType( TOKEN_SYMBOL_NAME );
                symbol.name = symbolName ? &symbolName->GetChild().GetText() : nullptr;

                symbol.isVariable = true;

                // if register is assigned explicitly
                const ParseTree * registerId = tree.GetFirstChildWithType( TOKEN_REGISTER_ID );
                if ( registerId )
                {
                    const char * registerName = registerId->GetText().c_str();
                    switch ( registerName[0] )
                    {
                    case 'b':
                        symbol.registerType = Symbol::REGISTER_TYPE_CBUFFER;
                        break;
                    case 't':
                        symbol.registerType = Symbol::REGISTER_TYPE_TEXTURE;
                        break;
                    case 's':
                        symbol.registerType = Symbol::REGISTER_TYPE_SAMPLER;
                        break;
                    case 'u':
                        symbol.registerType = Symbol::REGISTER_TYPE_UAV;
                        break;
                    default:
                        symbol.registerType = Symbol::REGISTER_TYPE_UNKNOWN;
                        CMN_FAIL();
                        return true;
                    }
                    symbol.registerIndex = std::atoi( ++ registerName );
                }

                AddSymbol( symbol );
            }
            return false;
        case TOKEN_FUNCTION_DEFINITION:
            {
                Symbol symbol;

                symbol.tree = &tree;
                const ParseTree * symbolName = tree.GetFirstChildWithType( TOKEN_SYMBOL_NAME );
                symbol.name = symbolName ? &symbolName->GetChild().GetText() : nullptr;

                symbol.isFunction = true;

                // Remember a return type

                symbol.type = &tree.GetChild();

                // Remember arguments

                const ParseTree * args = tree.GetFirstChildWithType( TOKEN_ARGUMENTS_LIST );
                if ( args )
                {
                    for ( uint32_t i = 0; i < args->GetChildCount(); ++ i )
                    {
                        if ( args->GetChild(i).GetTokenType() == TOKEN_ARGUMENT )
                        {
                            symbol.args.push_back( &args->GetChild(i) );
                        }
                    }
                }

                // Remember an output semantic

                const ParseTree * semantic = tree.GetFirstChildWithType( TOKEN_SEMANTIC );
                if ( semantic )
                {
                    symbol.semantic = &semantic->GetChild().GetText();
                }

                AddSymbol( symbol );
            }
            return false;
        case TOKEN_STATE_OBJECT_DEFINITION:
            {
                Symbol symbol;

                symbol.tree = &tree;
                const ParseTree * symbolName = tree.GetFirstChildWithType( TOKEN_SYMBOL_NAME );
                symbol.name = symbolName ? &symbolName->GetChild().GetText() : nullptr;

                symbol.isStateObject = true;
                symbol.isVariable = true;

                AddSymbol( symbol );
            }
            return false;
        case TOKEN_STRUCT_DEFINITION:
            {
                Symbol symbol;

                symbol.tree = &tree;
                const ParseTree * symbolName = tree.GetFirstChildWithType( TOKEN_SYMBOL_NAME );
                symbol.name = symbolName ? &symbolName->GetChild().GetText() : nullptr;

                symbol.isStruct = true;

                // Remember members

                for ( uint32_t i = 0; i < tree.GetChildCount(); ++ i )
                {
                    const ParseTree * child = &tree.GetChild(i);
                    if ( child->GetTokenType() == TOKEN_STRUCT_MEMBER_DECLARATION )
                    {
                        symbol.members.push_back( child );
                    }
                }

                AddSymbol( symbol );
            }
            return false;
        }
        return true;
    }

    bool SymbolTable::LookupSymbolByName( const std::string & name, SymbolReferenceVec & result ) const
    {
        Symbol symbol;
        symbol.name = &name;

        SymbolReferenceVec::const_iterator it = lower_bound( mSymbolsByName.begin(), mSymbolsByName.end(), &symbol, SymbolLessByName );
        if ( it != mSymbolsByName.end() )
        {
            for (; it != mSymbolsByName.end() && *(*it)->name == name; ++ it )
            {
                result.push_back( *it );
            }
        }

        return !result.empty();
    }

    const Symbol * SymbolTable::LookupSymbolByTree( const ParseTree & tree ) const
    {
        Symbol symbol;
        symbol.tree = &tree;

        SymbolReferenceVec::const_iterator it = lower_bound( mSymbolsByTreeAddress.begin(), mSymbolsByTreeAddress.end(),
            &symbol, SymbolLessByTreeAddress );

        return ( it != mSymbolsByTreeAddress.end() ) ? *it : NULL;
    }

    void SymbolTable::AddSymbol( const Symbol & symbol )
    {
        mSymbols.push_back( symbol );
    }

} // namesspace GFW
