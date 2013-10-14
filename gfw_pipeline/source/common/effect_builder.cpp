#include <fstream>

#include "common/trace.h"

#include "gfw_pipeline/effect_builder.h"

#include "FXLexer.h"
#include "FXParser.h"

namespace GFW { namespace Pipeline {

    EffectBinaryRef EffectBuilder::Build(const char * fileName)
    {
        pANTLR3_UINT8           fName = (pANTLR3_UINT8) fileName;
        pANTLR3_INPUT_STREAM    input = antlr3FileStreamNew(fName, ANTLR3_ENC_8BIT);
        TRACE_ASSERT(input != NULL);

        pFXLexer lxr = FXLexerNew(input);
        TRACE_ASSERT(lxr != NULL);

        pANTLR3_COMMON_TOKEN_STREAM tstream = antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT, TOKENSOURCE(lxr));
        TRACE_ASSERT(tstream != NULL);

        pFXParser psr = FXParserNew(tstream);
        TRACE_ASSERT(psr != NULL);

        FXParser_translation_unit_return ast = psr->translation_unit(psr);

        psr->free(psr);
        tstream->free(tstream);
        lxr->free(lxr);
        input->close(input);

        return NULL;
    }

}} // namespace GFW::Pipeline
