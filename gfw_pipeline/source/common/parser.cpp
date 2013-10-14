#include <unordered_map>

#include "common/trace.h"

#include "gfw_pipeline/parser.h"

#include "FXParser.h"

namespace GFW { namespace Pipeline {

    struct Shader
    {

    };
    typedef std::unordered_map < const char *, Shader > ShaderMap;

    struct Pass
    {
        Shader * vertexShader;
        Shader * pixelShader;
    };
    typedef std::unordered_map < const char *, Pass > PassMap;

    struct Technique
    {
        PassMap passes;
    };
    typedef std::unordered_map < const char *, Technique > TechniqueMap;

    struct ParserContext
    {
        ShaderMap       shaders;
        TechniqueMap    techniques;

        Technique *     currentTechnique;
        Pass *          currentPass;
    } gContext;

    void EnterTechnique( pANTLR3_STRING name )
    {
        TRACE_ASSERT( gContext.currentTechnique == NULL );
        gContext.currentTechnique = &gContext.techniques[ reinterpret_cast<char *>( name->chars ) ];
    }

    void LeaveTechnique()
    {
        TRACE_ASSERT( gContext.currentTechnique != NULL );
        gContext.currentTechnique = NULL;
    }

    void EnterPass( pANTLR3_STRING name )
    {
        TRACE_ASSERT( gContext.currentPass == NULL );
        gContext.currentPass = &gContext.currentTechnique->passes[ reinterpret_cast<char *>( name->chars ) ];
    }

    void LeavePass()
    {
        TRACE_ASSERT( gContext.currentPass != NULL );
        gContext.currentPass = NULL;
    }

    void SetShader( int token, pANTLR3_STRING shaderName )
    {
        TRACE_ASSERT( gContext.currentPass != NULL );

        switch ( token )
        {
        case T_SET_VERTEX_SHADER:
            break;
        case T_SET_PIXEL_SHADER:
            break;
        default:
            TRACE_FAIL();
            break;
        }
    }

}} // namespace GFW::Pipeline
