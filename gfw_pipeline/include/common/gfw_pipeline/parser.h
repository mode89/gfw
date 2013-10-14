#ifndef __GFW_PIPELINE_COMMON_PARSER_H__
#define __GFW_PIPELINE_COMMON_PARSER_H__

#include "antlr/antlr3.h"

namespace GFW { namespace Pipeline {

    void EnterTechnique( pANTLR3_STRING name );
    void LeaveTechnique();
    void EnterPass( pANTLR3_STRING name );
    void LeavePass();
    void SetShader( int token, pANTLR3_STRING shaderName );

}} // namespace GFW::Pipeline

#endif // __GFW_PIPELINE_COMMON_PARSER_H__
