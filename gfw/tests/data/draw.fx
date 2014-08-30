cbuffer cbXform
{
    float4x4    g_WVP : WORLDVIEWPROJ;
};

cbuffer cbColor
{
    float4      g_Color;
};

Texture2D       g_Texture;
SamplerState    g_Sampler;

float4 VS( float4 pos : POSITION ) : SV_POSITION
{
    return mul( pos, g_WVP );
}

float2 VS_Quad( float2 pos : POSITION ) : SV_POSITION
{
    return pos;
}

float4 PS_Colored() : SV_TARGET
{
    return g_Color;
}

struct VS_COLORED_VERTICES_INPUT
{
    float4 pos : POSITION;
    float4 color : COLOR;
};

struct VS_COLORED_VERTICES_OUTPUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

VS_COLORED_VERTICES_OUTPUT VS_ColoredVertices( VS_COLORED_VERTICES_INPUT inputs )
{
    VS_COLORED_VERTICES_OUTPUT outputs;
    outputs.pos = inputs.pos;
    outputs.color = inputs.color;
    return outputs;
}

float4 PS_ColoredVertices( VS_COLORED_VERTICES_OUTPUT inputs ) : SV_TARGET
{
    return inputs.color;
}

struct VS_TEXTURED_INPUT
{
    float4 pos : POSITION;
    float2 uv : TEXCOORD;
};

struct VS_TEXTURED_OUTPUT
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};

VS_TEXTURED_OUTPUT VS_Textured( VS_TEXTURED_INPUT inputs )
{
    VS_TEXTURED_OUTPUT outputs;

    outputs.pos = mul( inputs.pos, g_WVP );
    outputs.uv = inputs.uv;

    return outputs;
}

VS_TEXTURED_OUTPUT VS_TexturedQuad( float2 pos : POSITION )
{
    VS_TEXTURED_OUTPUT outputs;

    outputs.pos = float4( pos, 0.0f, 1.0f );
    outputs.uv = pos * float2( 0.5f, -0.5f ) + float2( 0.5f, 0.5f );

    return outputs;
}

float4 PS_Textured( VS_TEXTURED_OUTPUT inputs ) : SV_TARGET
{
    return g_Texture.Sample( g_Sampler, inputs.uv );
}

technique10 DrawColoredVertices
{
    pass p0
    {
        SetVertexShader( CompileShader( vs_4_0, VS_ColoredVertices() ) );
        SetPixelShader( CompileShader( ps_4_0, PS_ColoredVertices() ) );
    }
}

technique10 DrawColored
{
    pass p0
    {
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetPixelShader( CompileShader( ps_4_0, PS_Colored() ) );
    }
}

technique10 DrawTextured
{
    pass p0
    {
        SetVertexShader( CompileShader( vs_4_0, VS_Textured() ) );
        SetPixelShader( CompileShader( ps_4_0, PS_Textured() ) );
    }
}

technique10 DrawColoredQuad
{
    pass p0
    {
        SetVertexShader( CompileShader( vs_4_0, VS_Quad() ) );
        SetPixelShader( CompileShader( ps_4_0, PS_Colored() ) );
    }
}

technique10 DrawTexturedQuad
{
    pass p0
    {
        SetVertexShader( CompileShader( vs_4_0, VS_TexturedQuad() ) );
        SetPixelShader( CompileShader( ps_4_0, PS_Textured() ) );
    }
}

