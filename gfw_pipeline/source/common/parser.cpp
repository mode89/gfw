#include <list>
#include <unordered_map>

#include "common/trace.h"

#include "gfw_pipeline/parser.h"

#include "FXParser.h"

namespace GFW { namespace Pipeline {

    struct Argument
    {
        const char *    name;
        const char *    type;
        const char *    semantic;
    };
    typedef std::list < Argument > ArgumentList;

    struct Function
    {
        ArgumentList    args;
        const char *    returnType;
        const char *    returnSemantic;
    };
    typedef std::unordered_map < const char *, Function > FunctionMap;

    struct Pass
    {
        Function * vertexShader;
        Function * pixelShader;
    };
    typedef std::unordered_map < const char *, Pass > PassMap;

    struct Technique
    {
        PassMap passes;
    };
    typedef std::unordered_map < const char *, Technique > TechniqueMap;

    struct ParserContext
    {
        ArgumentList    args;
        FunctionMap     functions;
        FunctionMap     shaders;
        TechniqueMap    techniques;

        Function *      currentFunction;
        Technique *     currentTechnique;
        Pass *          currentPass;
    } gContext;

    void AddArgument( pANTLR3_STRING name, pANTLR3_STRING type, pANTLR3_STRING semantic )
    {
        TRACE_ASSERT( name != 0 );

        Argument arg;
        arg.name     = (const char *) name->chars;
        arg.type     = (const char *) type->chars;
        arg.semantic = ( semantic != NULL ) ? (const char *) semantic->chars : NULL;

        gContext.args.push_back( arg );
    }

    void EnterFunction( pANTLR3_STRING name, pANTLR3_STRING type, pANTLR3_STRING semantic )
    {
        TRACE_ASSERT( name != 0 );
        TRACE_ASSERT( gContext.currentFunction == NULL );
        Function * func = &gContext.functions[ reinterpret_cast<char *>( name->chars ) ];
        func->args           = gContext.args;
        func->returnType     = (const char *) type->chars;
        func->returnSemantic = ( semantic != NULL ) ? (const char *) semantic->chars : NULL;

        gContext.currentFunction = func;
        gContext.args.clear();
    }

    void LeaveFunction()
    {
        TRACE_ASSERT( gContext.currentFunction != NULL );
        gContext.currentFunction = NULL;
    }

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
