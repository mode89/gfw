#include <fstream>

#include "common/trace.h"

#include "gfw_pipeline/effect_builder.h"
#include "gfw_pipeline/parser.h"
#include "gfw_pipeline/parse_tree.h"

namespace GFW { namespace Pipeline {

    EffectBinaryRef EffectBuilder::Build(const char * fileName)
    {
        ParserRef parser  = new Parser( fileName );

        const ParseTree * tree = parser->GetTree();

        // Collect techniques

        return NULL;
    }

}} // namespace GFW::Pipeline
