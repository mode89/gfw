#include "common/trace.h"

#include "gfw_pipeline/parser.h"
#include "gfw_pipeline/parse_tree.h"

#include "FXLexer.h"
#include "FXParser.h"

namespace GFW { namespace Pipeline {

    struct ParserImpl
    {
        pFXLexer                        lexer;
        pANTLR3_COMMON_TOKEN_STREAM     tokenStream;
        pFXParser                       parser;
    };

    Parser::Parser( const char * fileName )
        : mImpl( NULL )
        , mTree( NULL )
    {
        ParserImpl * imp = new ParserImpl;
        mImpl = imp;

        pANTLR3_UINT8           fName = (pANTLR3_UINT8) fileName;
        pANTLR3_INPUT_STREAM    input = antlr3FileStreamNew( fName, ANTLR3_ENC_8BIT );
        TRACE_ASSERT( input != NULL );

        imp->lexer = FXLexerNew(input);
        TRACE_ASSERT( imp->lexer != NULL );

        imp->tokenStream = antlr3CommonTokenStreamSourceNew( ANTLR3_SIZE_HINT, TOKENSOURCE( imp->lexer ) );
        TRACE_ASSERT( imp->tokenStream != NULL);

        imp->parser = FXParserNew( imp->tokenStream );
        TRACE_ASSERT( imp->parser != NULL );

        FXParser_translation_unit_return ast = imp->parser->translation_unit( imp->parser );
        mTree = new ParseTree( ast.tree, this );

        input->close(input);
    }

    Parser::~Parser()
    {
        if ( mImpl != NULL )
        {
            ParserImpl * imp = static_cast<ParserImpl*>( mImpl );

            if ( imp->parser ) imp->parser->free( imp->parser );
            if ( imp->tokenStream ) imp->tokenStream->free( imp->tokenStream );
            if ( imp->lexer ) imp->lexer->free( imp->lexer );

            delete imp;
        }
    }

}} // namespace GFW::Pipeline
