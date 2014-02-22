#ifndef __GFW_PIPELINE_COMMON_PARSE_TREE_H__
#define __GFW_PIPELINE_COMMON_PARSE_TREE_H__

#include "common/autoref.h"

#include <vector>
#include <string>

namespace GFW {

#define TOKENS \
    T( ARGUMENT ) \
    T( ARGUMENTS_LIST ) \
    T( ARGUMENT_EXPRESSION_LIST ) \
    T( COMPILE_SHADER ) \
    T( COMPOUND_STATEMENT ) \
    T( EXTERNAL_DECLARATION ) \
    T( FUNCTION_DEFINITION ) \
    T( ID ) \
    T( PASS_DEFINITION ) \
    T( SAMPLER_OBJECT ) \
    T( SEMANTIC ) \
    T( SET_PIXEL_SHADER ) \
    T( SET_SHADER ) \
    T( SET_VERTEX_SHADER ) \
    T( STATE_OBJECT_DEFINITION ) \
    T( SYMBOL_NAME ) \
    T( TECHNIQUE_DEFINITION ) \
    T( TEXTURE_OBJECT ) \
    T( TEXTURE_SAMPLE_EXPRESSION ) \
    T( TRANSLATION_UNIT ) \
    T( VARIABLE_DEFINITION ) \
    \
    T( VOID ) \
    T( BOOL ) \
    T( INT ) \
    T( INT2 ) \
    T( INT3 ) \
    T( INT4 ) \
    T( UINT ) \
    T( UINT2 ) \
    T( UINT3 ) \
    T( UINT4 ) \
    T( HALF ) \
    T( HALF2 ) \
    T( HALF3 ) \
    T( HALF4 ) \
    T( FLOAT ) \
    T( FLOAT2 ) \
    T( FLOAT3 ) \
    T( FLOAT4 ) \
    T( FLOAT21 ) \
    T( FLOAT22 ) \
    T( FLOAT23 ) \
    T( FLOAT24 ) \
    T( FLOAT31 ) \
    T( FLOAT32 ) \
    T( FLOAT33 ) \
    T( FLOAT34 ) \
    T( FLOAT41 ) \
    T( FLOAT42 ) \
    T( FLOAT43 ) \
    T( FLOAT44 ) \

    enum TokenType
    {
        TOKEN_UNKNOWN = 0,
#define T(token) TOKEN_ ## token,
        TOKENS
#undef T
    };

    AUTOREF_FORWARD_DECLARATION( ParseTree );
    class ParseTree : public Common::ARefCounted
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
        TraverseDFS( Delegator & delegator, bool (Delegator::*visitor)( ConstParseTreeIn ) ) const
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

        uint32_t
        GetLine() const { return mLine; }

        uint32_t
        GetRow() const { return mRow; }

        uint32_t
        GetEndLine() const { return mEndLine; }

        uint32_t
        GetEndRow() const { return mEndRow; }

        ConstParseTreeRef
        GetChild( uint32_t index = 0 ) const { return mChildren[index]; }

        uint32_t
        GetChildCount() const { return mChildCount; }

        ConstParseTreeRef
        GetFirstChildWithType( TokenType ) const;

        const char *
        ToString() const { return mString.c_str(); }

    public:
        ParseTree( void * nativeTree );

    private:
        typedef std::vector< ParseTreeRef > ParseTreeVec;

        std::string     mString;
        TokenType       mTokenType;
        uint32_t        mLine;
        uint32_t        mRow;
        uint32_t        mEndLine;
        uint32_t        mEndRow;

        ParseTreeVec    mChildren;
        uint32_t        mChildCount;
    };

    ConstParseTreeRef
    CreateParseTree( const char * fileName );

} // namespace GFW

#endif // __GFW_PIPELINE_COMMON_PARSE_TREE_H__
