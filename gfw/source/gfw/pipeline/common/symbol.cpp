#include "cmn/trace.h"
#include "gfw/pipeline/common/parse_tree.h"
#include "gfw/pipeline/common/symbol.h"
#include <algorithm>
#include <cstring>

namespace GFW {

    static bool CollectSymbol( const ParseTree &, SymbolTable & );

    class CollectReferencedSymbolsVisitor
    {
    public:
        CollectReferencedSymbolsVisitor( const SymbolTable & symbolTable, NameSymbolMap & references )
            : mReferences( references )
        {
            // Map symbols by name
            for ( auto & symbol : symbolTable )
            {
                mNameSymbolMap.emplace( std::make_pair( symbol.name, &symbol ) );
            }
        }

        bool operator() ( const ParseTree & tree )
        {
            if ( tree.GetTokenType() == TOKEN_ID )
            {
                auto range = mNameSymbolMap.equal_range( tree.GetText() );
                mReferences.insert( range.first, range.second );
            }
            return true;
        }

        CollectReferencedSymbolsVisitor operator= ( const CollectReferencedSymbolsVisitor & );

    private:
        NameSymbolMap   mNameSymbolMap;
        NameSymbolMap & mReferences;
    };

    void ConstructSymbolTable( SymbolTable & symbolTable, const ParseTree & tree )
    {
        tree.TraverseDFS( CollectSymbol, symbolTable );

        // Collect immediate references (variable by function, function by function, etc)
        for ( auto & symbol : symbolTable )
        {
            symbol.references.emplace( std::make_pair( symbol.name, &symbol ) );
            switch ( symbol.tree->GetTokenType() )
            {
            case TOKEN_FUNCTION_DEFINITION:
                {
                    CollectReferencedSymbolsVisitor collectReferencedSymbolsVisitor( symbolTable, symbol.references );
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
        for ( auto & symbol : symbolTable )
        {
            // Start with immediate references
            NameSymbolMap newReferences = symbol.references;
            while ( !newReferences.empty() )
            {
                // Enumerate through new references
                NameSymbolMap nextNewReferences;
                for ( auto & processing : newReferences )
                {
                    // Enumerate through immediate references of the new references
                    for ( auto & indirect : processing.second->references )
                    {
                        // If the reference is not known then we will process it during the next iteration
                        if ( symbol.references.count( indirect.second->name ) == 0 )
                        {
                            nextNewReferences.insert( indirect );
                        }
                    }
                }
                newReferences = nextNewReferences;

                // Add new references to the known references
                symbol.references.insert( nextNewReferences.begin(), nextNewReferences.end() );
            }
        }
    }

    bool CollectSymbol( const ParseTree & tree, SymbolTable & symbolTable )
    {
        switch ( tree.GetTokenType() )
        {
        case TOKEN_VARIABLE_DEFINITION:
            {
                Symbol symbol;

                symbol.tree = &tree;
                const ParseTree * symbolName = tree.GetFirstChildWithType( TOKEN_SYMBOL_NAME );
                symbol.name = symbolName ? symbolName->GetChild().GetText().c_str() : nullptr;

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

                switch ( tree.GetChild().GetTokenType() )
                {
                case TOKEN_TEXTURE2D:
                    symbol.isTexture = true;
                    break;
                case TOKEN_SAMPLERSTATE:
                    symbol.isSamplerState = true;
                    break;
                }

                symbolTable.push_back( symbol );
            }
            return false;
        case TOKEN_FUNCTION_DEFINITION:
            {
                Symbol symbol;

                symbol.tree = &tree;
                const ParseTree * symbolName = tree.GetFirstChildWithType( TOKEN_SYMBOL_NAME );
                symbol.name = symbolName ? symbolName->GetChild().GetText().c_str() : nullptr;

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
                symbol.semantic = semantic ? semantic->GetChild().GetText().c_str() : nullptr;

                symbolTable.push_back( symbol );
            }
            return false;
        case TOKEN_STATE_OBJECT_DEFINITION:
            {
                Symbol symbol;

                symbol.tree = &tree;
                const ParseTree * symbolName = tree.GetFirstChildWithType( TOKEN_SYMBOL_NAME );
                symbol.name = symbolName ? symbolName->GetChild().GetText().c_str() : nullptr;

                symbol.isStateObject = true;
                symbol.isVariable = true;

                symbolTable.push_back( symbol );
            }
            return false;
        case TOKEN_STRUCT_DEFINITION:
            {
                Symbol symbol;

                symbol.tree = &tree;
                const ParseTree * symbolName = tree.GetFirstChildWithType( TOKEN_SYMBOL_NAME );
                symbol.name = symbolName ? symbolName->GetChild().GetText().c_str() : nullptr;

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

                symbolTable.push_back( symbol );
            }
            return false;
        }
        return true;
    }

} // namesspace GFW
