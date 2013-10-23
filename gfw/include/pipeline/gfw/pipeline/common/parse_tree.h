#ifndef __GFW_PIPELINE_COMMON_PARSE_TREE_H__
#define __GFW_PIPELINE_COMMON_PARSE_TREE_H__

#include "common/autoref.h"

namespace GFW {

#define TOKENS \
    T(TRANSLATION_UNIT) \
    T(EXTERNAL_DECLARATION) \
    T(FUNCTION_DEFINITION) \
    T(TECHNIQUE_DEFINITION) \
    T(PASS_DEFINITION) \
    T(ARGUMENTS_LIST) \
    T(ARGUMENT) \
    T(SEMANTIC) \
    T(SET_SHADER) \
    T(SET_VERTEX_SHADER) \
    T(SET_PIXEL_SHADER) \
    T(COMPILE_SHADER) \
    T(ID) \

    enum TokenType
    {
        TOKEN_UNKNOWN = 0,
#define T(token) TOKEN_ ## token,
        TOKENS
#undef T
    };

    struct ParseTreeImpl;
    AUTOREF_FORWARD_DECLARATION( Parser );

    class ParseTree
    {
    public:
        template < class Visitor > void
        TraverseDFS( Visitor & visitor ) const
        {
            if ( visitor( this ) )
            {
                for ( uint32_t i = 0; i < mChildCount; ++ i )
                {
                    mChildren[i]->TraverseDFS( visitor );
                }
            }
        }

        template < class Delegator > void
        TraverseDFS( Delegator & delegator, bool (Delegator::*visitor)( const ParseTree * ) ) const
        {
            if ( (delegator.*visitor)( this ) )
            {
                for ( uint32_t i = 0; i < mChildCount; ++ i )
                {
                    mChildren[i]->TraverseDFS( delegator, visitor );
                }
            }
        }

        TokenType
        GetTokenType() const { return mTokenType; }

        const ParseTree *
        GetChild( uint32_t index = 0 ) const { return mChildren[index]; }

        const char *
        ToString() const { return mString; }

    public:
        ParseTree( void * nativeTree, ParserIn );
        ~ParseTree();

    private:
        ParseTreeImpl * mImpl;
        ParserRef       mParser;

        const char *    mString;
        TokenType       mTokenType;

        ParseTree **    mChildren;
        uint32_t        mChildCount;
    };

} // namespace GFW

#endif // __GFW_PIPELINE_COMMON_PARSE_TREE_H__
