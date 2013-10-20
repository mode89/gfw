#version 120

varying   vec2 out_TexCoord;

#if defined(GFW_SHADER_STAGE_VERTEX)

attribute vec2 POSITION0;

void main()
{
    gl_Position  = vec4(POSITION0, 0.0f, 1.0f);
    out_TexCoord = POSITION0 * vec2(0.5f, -0.5f) + vec2(0.5f, 0.5f);
}

#elif defined(GFW_SHADER_STAGE_PIXEL)

uniform sampler2D   g_Texture;

void main()
{
    gl_FragColor = texture2D(g_Texture, out_TexCoord);
}

#endif
