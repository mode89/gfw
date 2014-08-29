#include "cmn/trace.h"
#include "gfw/pipeline/common/effect_builder_exception.h"
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
            CollectReferencedSymbolsVisitor collectReferencedSymbolsVisitor( symbolTable, symbol.references );
            symbol.tree->TraverseDFS( collectReferencedSymbolsVisitor );
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

        // Collect indirect references to cbuffers
        for ( auto & symbol : symbolTable )
        {
            if ( symbol.isFunction )
            {
                NameSymbolMap & references = symbol.references;
                for ( auto & referredSymbol : references )
                {
                    if ( referredSymbol.second->isCbufferMember )
                    {
                        const Symbol * cbufferSymbol = referredSymbol.second->parent;
                        if ( references.count( cbufferSymbol->name ) == 0 )
                        {
                            references.insert( std::make_pair( cbufferSymbol->name, cbufferSymbol ) );
                        }
                    }
                }
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
                const ParseTree * registerBinding = tree.GetFirstChildWithType( TOKEN_REGISTER_BINDING );
                if ( registerBinding )
                {
                    const ParseTree * registerId = registerBinding->GetFirstChildWithType( TOKEN_REGISTER_ID );
                    CMN_ASSERT( registerId != nullptr );

                    const char * registerName = registerId->GetText().c_str();
                    switch ( registerName[0] )
                    {
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
                        CMN_THROW( EffectBuilderException::WrongRegister( symbol.name ) );
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
        case TOKEN_CBUFFER_DEFINITION:
            {
                Symbol symbol;

                symbol.tree = &tree;
                const ParseTree * symbolName = tree.GetFirstChildWithType( TOKEN_SYMBOL_NAME );
                symbol.name = symbolName ? symbolName->GetChild().GetText().c_str() : nullptr;

                symbol.isCbuffer = true;

                // if register is assigned explicitly
                const ParseTree * registerBinding = tree.GetFirstChildWithType( TOKEN_REGISTER_BINDING );
                if ( registerBinding )
                {
                    const ParseTree * registerId = registerBinding->GetFirstChildWithType( TOKEN_REGISTER_ID );
                    CMN_ASSERT( registerId != nullptr );

                    const char * registerName = registerId->GetText().c_str();
                    if ( registerName[ 0 ] == 'b' )
                    {
                        symbol.registerType = Symbol::REGISTER_TYPE_CBUFFER;
                        symbol.registerIndex = std::atoi( ++ registerName );
                    }
                    else
                    {
                        CMN_THROW( EffectBuilderException::WrongRegister( symbol.name ) );
                        return true;
                    }
                }

                symbolTable.push_back( symbol );
                const Symbol * cbufferSymbol = &symbolTable.back();

                // Collect members
                const ParseTree * memberList = tree.GetFirstChildWithType( TOKEN_CBUFFER_MEMBER_LIST );
                CMN_THROW_IF( memberList == nullptr, EffectBuilderException::EmptyCbuffer( symbol.name ) );
                for ( uint32_t i = 0; i < memberList->GetChildCount(); ++ i )
                {
                    Symbol memberSymbol;

                    const ParseTree & member = memberList->GetChild( i );
                    memberSymbol.tree = &member;

                    const ParseTree * memberName = member.GetFirstChildWithType( TOKEN_SYMBOL_NAME );
                    memberSymbol.name = memberName->GetChild().GetText().c_str();

                    const ParseTree * semantic = member.GetFirstChildWithType( TOKEN_SEMANTIC );
                    memberSymbol.semantic = semantic ? semantic->GetChild().GetText().c_str() : nullptr;

                    memberSymbol.isCbufferMember = true;
                    memberSymbol.parent = cbufferSymbol;

                    symbolTable.push_back( memberSymbol );
                }
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
