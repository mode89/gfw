#include "cmn/trace.h"
#include "gfw/pipeline/common/effect_builder_exception.h"
#include "gfw/pipeline/common/parse_tree.h"
#include "FXLexer.h"
#include "FXParser.h"

namespace GFW {

    static TokenType GetTokenType( uint32_t nativeType )
    {
        switch ( nativeType )
        {
#define T(token) case T_ ## token : return TOKEN_ ## token;
            TOKENS
#undef T
        default:
            break;
        }

        return TOKEN_UNKNOWN;
    }

    std::shared_ptr< const ParseTree > CreateParseTree( const boost::filesystem::path & filePath )
    {
        pANTLR3_UINT8 filePathString = (pANTLR3_UINT8) filePath.c_str();

        std::shared_ptr< ANTLR3_INPUT_STREAM > inputStream(
            antlr3FileStreamNew( filePathString, ANTLR3_ENC_8BIT ),
            [] ( ANTLR3_INPUT_STREAM * stream ) { stream->close( stream ); } );
        CMN_THROW_IF( !inputStream, EffectBuilderException::ParsingError() );

        std::shared_ptr< FXLexer > lexer(
            FXLexerNew( inputStream.get() ),
            [] ( FXLexer * lexer ) { lexer->free( lexer ); } );
        CMN_THROW_IF( !lexer, EffectBuilderException::ParsingError() );

        std::shared_ptr< ANTLR3_COMMON_TOKEN_STREAM > tokenStream(
            antlr3CommonTokenStreamSourceNew( ANTLR3_SIZE_HINT, TOKENSOURCE( lexer.get() ) ),
            [] ( ANTLR3_COMMON_TOKEN_STREAM * stream ) { stream->free( stream ); } );
        CMN_THROW_IF( !tokenStream, EffectBuilderException::ParsingError() );

        std::shared_ptr< FXParser > parser(
            FXParserNew( tokenStream.get() ),
            [] ( FXParser * parser ) { parser->free( parser ); } );
        CMN_THROW_IF( !parser, EffectBuilderException::ParsingError() );

        FXParser_translation_unit_return ast = parser->translation_unit( parser.get() );
        CMN_THROW_IF( parser->pParser->rec->state->errorCount != 0, EffectBuilderException::ParsingError() );

        return std::make_shared< ParseTree >( ast.tree );
    }

    ParseTree::ParseTree( void * nativeTree )
        : mTokenType( TOKEN_UNKNOWN )
        , mLine( 0 )
        , mRow( 0 )
        , mEndLine( 0 )
        , mEndRow( 0 )
        , mChildCount( 0 )
    {
        pANTLR3_BASE_TREE tree = static_cast<pANTLR3_BASE_TREE>( nativeTree );

        pANTLR3_COMMON_TOKEN token = tree->getToken( tree );
        mTokenType = GFW::GetTokenType( token->getType( token ) );
        if ( token->getLine( token ) )
        {
            mString = reinterpret_cast< const char * >( token->getText( token )->chars );

            mLine = token->getLine( token );
            mEndLine = mLine;

            mRow = tree->getCharPositionInLine( tree );

            // FIXME Strange behavior of ANTLR
            if ( mLine == 1 && mRow > 0 )
            {
                mRow ++;
            }

            mEndRow = mRow + mString.size();
        }

        mChildCount = tree->getChildCount( tree );
        mChildren.reserve( mChildCount );
        for ( uint32_t i = 0; i < mChildCount; ++ i )
        {
            mChildren.emplace_back( tree->getChild( tree, i ) );
        }

        if ( mChildCount > 0 )
        {
            mLine = mChildren[0].GetLine();
            mRow  = mChildren[0].GetRow();

            mEndLine = mChildren.back().GetEndLine();
            mEndRow  = mChildren.back().GetEndRow();
        }
    }

    const ParseTree * ParseTree::GetFirstChildWithType( TokenType type ) const
    {
        for ( uint32_t i = 0; i < mChildCount; ++ i )
        {
            const ParseTree & child = mChildren[i];
            if ( child.GetTokenType() == type )
            {
                return &child;
            }
        }

        return nullptr;
    }

} // namespace GFW
