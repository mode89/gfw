#include "cmn/trace.h"
#include "gfw/pipeline/common/parse_tree.h"
#include "gfw/pipeline/common/symbol_table.h"
#include <algorithm>
#include <cstring>

namespace GFW {

    static bool SymbolLessByName( const Symbol * l, const Symbol * r )
    {
        return l->GetName() < r->GetName();
    }

    static bool SymbolLessByTreeAddress( const Symbol * l, const Symbol * r )
    {
        return &l->GetTree() < &r->GetTree();
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
        return std::binary_search( mReferences.begin(), mReferences.end(), symbol );
    }

    SymbolTable::SymbolTable( const ParseTree & tree )
    {
        tree.TraverseDFS( *this, &SymbolTable::CollectSymbol );

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
                    CollectReferencedSymbolsVisitor collectReferencedSymbolsVisitor( *this, symbol.mReferences );
                    symbol.GetTree().TraverseDFS( collectReferencedSymbolsVisitor );
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

            // Sort references to be able to search through them
            std::sort( symbol.mReferences.begin(), symbol.mReferences.end() );
        }
    }

    bool SymbolTable::CollectSymbol( const ParseTree & tree )
    {
        switch ( tree.GetTokenType() )
        {
        case TOKEN_VARIABLE_DEFINITION:
            {
                Symbol symbol( tree );
                symbol.SetVariable();

                // if register is assigned explicitly
                const ParseTree * registerId = tree.GetFirstChildWithType( TOKEN_REGISTER_ID );
                if ( registerId )
                {
                    const char * registerName = registerId->GetText().c_str();
                    switch ( registerName[0] )
                    {
                    case 'b':
                        symbol.mRegisterType = Symbol::REGISTER_TYPE_CBUFFER;
                        break;
                    case 't':
                        symbol.mRegisterType = Symbol::REGISTER_TYPE_TEXTURE;
                        break;
                    case 's':
                        symbol.mRegisterType = Symbol::REGISTER_TYPE_SAMPLER;
                        break;
                    case 'u':
                        symbol.mRegisterType = Symbol::REGISTER_TYPE_UAV;
                        break;
                    default:
                        symbol.mRegisterType = Symbol::REGISTER_TYPE_UNKNOWN;
                        CMN_FAIL();
                        return true;
                    }
                    symbol.mRegisterIndex = std::atoi( ++ registerName );
                }

                AddSymbol( symbol );
            }
            return false;
        case TOKEN_FUNCTION_DEFINITION:
            {
                Symbol symbol( tree );
                symbol.SetFunction();

                // Remember a return type

                symbol.mType = &tree.GetChild();

                // Remember arguments

                const ParseTree * args = tree.GetFirstChildWithType( TOKEN_ARGUMENTS_LIST );
                if ( args )
                {
                    for ( uint32_t i = 0; i < args->GetChildCount(); ++ i )
                    {
                        if ( args->GetChild(i).GetTokenType() == TOKEN_ARGUMENT )
                        {
                            symbol.mArgs.push_back( &args->GetChild(i) );
                        }
                    }
                }

                // Remember an output semantic

                const ParseTree * semantic = tree.GetFirstChildWithType( TOKEN_SEMANTIC );
                if ( semantic )
                {
                    symbol.mSemantic = &semantic->GetChild().GetText();
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
        case TOKEN_STRUCT_DEFINITION:
            {
                Symbol symbol( tree );
                symbol.SetStruct();

                // Remember members

                for ( uint32_t i = 0; i < tree.GetChildCount(); ++ i )
                {
                    const ParseTree * child = &tree.GetChild(i);
                    if ( child->GetTokenType() == TOKEN_STRUCT_MEMBER_DECLARATION )
                    {
                        symbol.mMembers.push_back( child );
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
        symbol.mName = &name;

        SymbolReferenceVec::const_iterator it = lower_bound( mSymbolsByName.begin(), mSymbolsByName.end(), &symbol, SymbolLessByName );
        if ( it != mSymbolsByName.end() )
        {
            for (; it != mSymbolsByName.end() && (*it)->GetName() == name; ++ it )
            {
                result.push_back( *it );
            }
        }

        return !result.empty();
    }

    const Symbol * SymbolTable::LookupSymbolByTree( const ParseTree & tree ) const
    {
        Symbol symbol;
        symbol.mTree = &tree;

        SymbolReferenceVec::const_iterator it = lower_bound( mSymbolsByTreeAddress.begin(), mSymbolsByTreeAddress.end(),
            &symbol, SymbolLessByTreeAddress );

        return ( it != mSymbolsByTreeAddress.end() ) ? *it : NULL;
    }

    void SymbolTable::AddSymbol( const Symbol & symbol )
    {
        mSymbols.push_back( symbol );
    }

} // namesspace GFW
