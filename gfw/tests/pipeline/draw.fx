float4 VS( float4 pos : POSITION ) : SV_POSITION
{
    return pos;
}

float4 PS() : SV_TARGET
{
    return float4(1.0f, 0.0f, 0.0f, 1.0f);
}

technique Draw
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}