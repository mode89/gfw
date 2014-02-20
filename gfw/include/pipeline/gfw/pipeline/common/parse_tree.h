#ifndef __GFW_PIPELINE_COMMON_PARSE_TREE_H__
#define __GFW_PIPELINE_COMMON_PARSE_TREE_H__

#include "common/autoref.h"

#include <vector>
#include <string>

namespace GFW {

    #include "FXTokens.h"

    enum TokenType
    {
        T_UNKNOWN = 0,
#define T(token, num) token = num,
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
