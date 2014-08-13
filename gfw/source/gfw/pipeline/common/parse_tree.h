#ifndef __GFW_PIPELINE_COMMON_PARSE_TREE_H__
#define __GFW_PIPELINE_COMMON_PARSE_TREE_H__

#include "cmn/platform.h"

CMN_WARNING_PUSH
CMN_WARNING_DISABLE_MSVC( 4548 4625 4626 4640 )
#include "boost/filesystem.hpp"
CMN_WARNING_POP

#include <memory>
#include <string>
#include <vector>

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
    T( REGISTER_ID ) \
    T( SAMPLER_OBJECT_ID ) \
    T( SAMPLERSTATE ) \
    T( SEMANTIC ) \
    T( SET_PIXEL_SHADER ) \
    T( SET_SHADER ) \
    T( SET_VERTEX_SHADER ) \
    T( STATE_OBJECT_DEFINITION ) \
    T( STRUCT_DEFINITION ) \
    T( STRUCT_MEMBER_DECLARATION ) \
    T( SYMBOL_NAME ) \
    T( TECHNIQUE_DEFINITION ) \
    T( TEXTURE2D ) \
    T( TEXTURE_OBJECT_ID ) \
    T( TEXTURE_SAMPLE_EXPRESSION ) \
    T( TEXTURE_SAMPLE_EXPRESSION_HEAD ) \
    T( TRANSLATION_UNIT ) \
    T( TYPE_SPECIFIER ) \
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

    class ParseTree
    {
    public:
        template < class Visitor > void
        TraverseDFS( Visitor & visitor ) const
        {
            if ( visitor( *this ) )
            {
                for ( uint32_t i = 0; i < mChildCount; ++ i )
                {
                    mChildren[i].TraverseDFS( visitor );
                }
            }
        }

        template < typename Callback, typename Context > void
        TraverseDFS( Callback & callback, Context & context ) const
        {
            if ( callback( *this, context ) )
            {
                for ( uint32_t i = 0; i < mChildCount; ++ i )
                {
                    mChildren[i].TraverseDFS( callback, context );
                }
            }
        }

        template < class Delegator > void
        TraverseDFS( Delegator & delegator, bool (Delegator::*visitor)( const ParseTree & ) ) const
        {
            if ( (delegator.*visitor)( *this ) )
            {
                for ( uint32_t i = 0; i < mChildCount; ++ i )
                {
                    mChildren[i].TraverseDFS( delegator, visitor );
                }
            }
        }

        TokenType
        GetTokenType() const { return mTokenType; }

        size_t
        GetLine() const { return mLine; }

        size_t
        GetRow() const { return mRow; }

        size_t
        GetEndLine() const { return mEndLine; }

        size_t
        GetEndRow() const { return mEndRow; }

        const ParseTree &
        GetChild( uint32_t index = 0 ) const { return mChildren[index]; }

        uint32_t
        GetChildCount() const { return mChildCount; }

        const ParseTree *
        GetFirstChildWithType( TokenType ) const;

        const std::string &
        GetText() const { return mString; }

    public:
        ParseTree( void * nativeTree );

    private:
        typedef std::vector< ParseTree > ParseTreeVec;

        std::string     mString;
        TokenType       mTokenType;
        size_t          mLine;
        size_t          mRow;
        size_t          mEndLine;
        size_t          mEndRow;

        ParseTreeVec    mChildren;
        uint32_t        mChildCount;
    };

    std::shared_ptr< const ParseTree >
    CreateParseTree( const boost::filesystem::path & );

} // namespace GFW

#endif // __GFW_PIPELINE_COMMON_PARSE_TREE_H__
