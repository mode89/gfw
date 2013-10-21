#include "common/trace.h"

#include "gfw/pipeline/common/parser.h"
#include "gfw/pipeline/common/parse_tree.h"

#include "FXLexer.h"
#include "FXParser.h"

namespace GFW {

    struct ParserImpl
    {
        pANTLR3_INPUT_STREAM            inputStream;
        pFXLexer                        lexer;
        pANTLR3_COMMON_TOKEN_STREAM     tokenStream;
        pFXParser                       parser;
    };

    Parser::Parser( const char * fileName )
        : mImpl( NULL )
        , mTree( NULL )
    {
        mImpl = new ParserImpl;

        pANTLR3_UINT8 fName = (pANTLR3_UINT8) fileName;
        mImpl->inputStream = antlr3FileStreamNew( fName, ANTLR3_ENC_8BIT );
        TRACE_ASSERT( mImpl->inputStream != NULL );

        mImpl->lexer = FXLexerNew( mImpl->inputStream );
        TRACE_ASSERT( mImpl->lexer != NULL );

        mImpl->tokenStream = antlr3CommonTokenStreamSourceNew( ANTLR3_SIZE_HINT, TOKENSOURCE( mImpl->lexer ) );
        TRACE_ASSERT( mImpl->tokenStream != NULL);

        mImpl->parser = FXParserNew( mImpl->tokenStream );
        TRACE_ASSERT( mImpl->parser != NULL );

        FXParser_translation_unit_return ast = mImpl->parser->translation_unit( mImpl->parser );
        mTree = new ParseTree( ast.tree, this );
    }

    Parser::~Parser()
    {
        if ( mTree != NULL )
        {
            delete mTree;
        }

        if ( mImpl != NULL )
        {
            if ( mImpl->parser ) mImpl->parser->free( mImpl->parser );
            if ( mImpl->tokenStream ) mImpl->tokenStream->free( mImpl->tokenStream );
            if ( mImpl->lexer ) mImpl->lexer->free( mImpl->lexer );
            if ( mImpl->inputStream ) mImpl->inputStream->close( mImpl->inputStream );

            delete mImpl;
        }
    }

} // namespace GFW
