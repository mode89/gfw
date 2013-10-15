#include <list>
#include <string>
#include <unordered_map>

#include "common/trace.h"

#include "gfw_pipeline/parser.h"

#include "FXParser.h"

namespace GFW { namespace Pipeline {

    struct Argument
    {
        std::string name;
        std::string type;
        std::string semantic;
    };
    typedef std::list < Argument > ArgumentList;

    struct Function
    {
        ArgumentList args;
        std::string  returnType;
        std::string  returnSemantic;
    };
    typedef std::unordered_map < std::string, Function > FunctionMap;

    struct Pass
    {
        Function * vertexShader;
        Function * pixelShader;
    };
    typedef std::unordered_map < std::string, Pass > PassMap;

    struct Technique
    {
        PassMap passes;
    };
    typedef std::unordered_map < std::string, Technique > TechniqueMap;

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
        arg.name     = reinterpret_cast< char * >( name->chars );
        arg.type     = reinterpret_cast< char * >( type->chars );
        arg.semantic = ( semantic != NULL ) ? reinterpret_cast< char * >( semantic->chars ) : NULL;

        gContext.args.push_back( arg );
    }

    void EnterFunction( pANTLR3_STRING name, pANTLR3_STRING type, pANTLR3_STRING semantic )
    {
        TRACE_ASSERT( name != 0 );
        TRACE_ASSERT( gContext.currentFunction == NULL );
        Function * func = &gContext.functions[ reinterpret_cast< char * >( name->chars ) ];
        func->args           = gContext.args;
        func->returnType     = reinterpret_cast< char * >( type->chars );
        func->returnSemantic = ( semantic != NULL ) ? reinterpret_cast< char * >( semantic->chars ) : NULL;

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
        gContext.currentTechnique = &gContext.techniques[ reinterpret_cast< char * >( name->chars ) ];
    }

    void LeaveTechnique()
    {
        TRACE_ASSERT( gContext.currentTechnique != NULL );
        ParserContext * ctx = &gContext;
        gContext.currentTechnique = NULL;
    }

    void EnterPass( pANTLR3_STRING name )
    {
        TRACE_ASSERT( gContext.currentPass == NULL );
        gContext.currentPass = &gContext.currentTechnique->passes[ reinterpret_cast< char * >( name->chars ) ];
    }

    void LeavePass()
    {
        TRACE_ASSERT( gContext.currentPass != NULL );
        gContext.currentPass = NULL;
    }

    void SetShader( int token, pANTLR3_STRING shaderName )
    {
        TRACE_ASSERT( gContext.currentPass != NULL );
        Pass * pass = gContext.currentPass;

        ParserContext * ctx = &gContext;
        FunctionMap::iterator it = gContext.functions.find( reinterpret_cast< char * >( shaderName->chars ) );
        TRACE_ASSERT( it != gContext.functions.end() );
        Function * shader = &it->second;

        switch ( token )
        {
        case T_SET_VERTEX_SHADER:
            pass->vertexShader = shader;
            break;
        case T_SET_PIXEL_SHADER:
            pass->pixelShader = shader;
            break;
        default:
            TRACE_FAIL();
            break;
        }
    }

}} // namespace GFW::Pipeline
