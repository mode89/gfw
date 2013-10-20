#include <fstream>

#include "common/trace.h"

#include "gfw/pipeline/common/effect_builder.h"
#include "gfw/pipeline/common/parser.h"
#include "gfw/pipeline/common/parse_tree.h"

namespace GFW { namespace Pipeline {

    EffectBinaryRef EffectBuilder::Build(const char * fileName)
    {
        ParserRef parser  = new Parser( fileName );

        const ParseTree * tree = parser->GetTree();

        // Collect techniques

        return NULL;
    }

}} // namespace GFW::Pipeline
